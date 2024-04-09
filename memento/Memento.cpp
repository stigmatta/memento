#include<iostream>
#include<vector>
#include <string>
#include<ctime>
#include<windows.h>
using namespace std;

class Memento
{
public:
    virtual string GetText() const = 0;
    virtual string GetCursorPos() const = 0;
    virtual string GetSelection() const = 0;
    virtual string GetCurrentFont() const = 0;
    virtual string GetStyle() const = 0;
};

class ConcreteMemento : public Memento
{
private:
    string text;
    string cursorPos;
    string selection;
    string currentFont;
    string style;

public:
    ConcreteMemento(string state, string cursorPos, string selection, string currentFont, string style)
        : text(state), cursorPos(cursorPos), selection(selection), currentFont(currentFont), style(style) {}

    string GetCursorPos() const override
    {
        return cursorPos;
    }

    string GetText() const override
    {
        return text;
    }

    string GetSelection() const override
    {
        return selection;
    }

    string GetCurrentFont() const override
    {
        return currentFont;
    }

    string GetStyle() const override
    {
        return style;
    }
};

class Originator
{
private:
    string text;

    string GenerateRandomString(int length = 10)
    {
        const char alphanum[] =
            "Hello world"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        int stringLength = sizeof(alphanum) - 1;

        string random_string;
        for (int i = 0; i < length; i++) {
            random_string += alphanum[rand() % stringLength];
        }
        return random_string;
    }

    string GenerateRandomFont() {
        const vector<string> fonts = { "Arial", "Times New Roman", "Verdana", "Courier New", "Tahoma" };
        return fonts[rand() % fonts.size()];
    }

    string GenerateRandomSelection() {
        const vector<string> selections = { "Bold", "Italic", "Underline", "None" };
        return selections[rand() % selections.size()];
    }

    string GenerateRandomStyle() {
        const vector<string> styles = { "Normal", "Heading", "Subtitle", "Quote" };
        return styles[rand() % styles.size()];
    }

public:
    Originator(string state) : text(state) {
        cout << "Originator: My initial state is: " << text << "\n";
    }

    void DoSomething() {
        cout << "Originator: I'm doing something important.\n";
        text = GenerateRandomString(30);
        cout << "Originator: and my state has changed to: " << text << "\n";
    }

    Memento* Save() {
        time_t now = time(0);
        string randomFont = GenerateRandomFont();
        string randomSelection = GenerateRandomSelection();
        string randomStyle = GenerateRandomStyle();
        return new ConcreteMemento(text, to_string(now), randomSelection, randomFont, randomStyle);
    }

    void Restore(Memento* memento) {
        text = memento->GetText();
        cout << "Originator: My state has changed to: " << text << "\n";
    }
};

class Caretaker
{
private:
    vector<Memento*> mementos_;
    Originator* originator_;

public:
    Caretaker(Originator* originator) : originator_(originator) {}

    void Backup()
    {
        cout << "\nCaretaker: Saving Originator's state...\n";
        mementos_.push_back(originator_->Save());
    }

    void Undo()
    {
        if (!mementos_.empty())
        {
            Memento* memento = mementos_.back();
            mementos_.pop_back();
            cout << "Caretaker: Restoring state to: " << memento->GetCursorPos() << "\n";
            cout << "Text: " << memento->GetText() << "\n";
            cout << "Selection: " << memento->GetSelection() << "\n";
            cout << "Font: " << memento->GetCurrentFont() << "\n";
            originator_->Restore(memento);
            delete memento;
        }
    }

    void ShowHistory() const
    {
        cout << "Caretaker: Here's the list of mementos:\n";
        for (auto memento : mementos_)
        {
            cout << "Cursor Pos: " << memento->GetCursorPos() << "\n";
            cout << "Text: " << memento->GetText() << "\n";
            cout << "Selection: " << memento->GetSelection() << "\n";
            cout << "Font: " << memento->GetCurrentFont() << "\n";
            cout << "Style: " << memento->GetStyle() << "\n";
            cout << "-----------------------------------------\n";
        }
    }

    ~Caretaker()
    {
        for (auto memento : mementos_)
        {
            delete memento;
        }
        mementos_.clear();
    }
};

void ClientCode()
{
    Originator* originator = new Originator("Super-duper-super-puper-super.");
    Caretaker* caretaker = new Caretaker(originator);
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    caretaker->Backup();
    originator->DoSomething();
    cout << "\n";
    caretaker->ShowHistory();
    cout << "\nClient: Now, let's rollback!\n\n";
    caretaker->Undo();
    cout << "\nClient: Once more!\n\n";
    caretaker->Undo();

    cout << "\n-------------------------------------------\n";
    caretaker->ShowHistory();
    delete originator;
    delete caretaker;
}

int main()
{
    srand((unsigned)time(NULL));
    ClientCode();
    return 0;
}