#ifndef MENUMODEL_H
#define MENUMODEL_H

enum class MenuState {MAIN, SETTINGS};

class MenuModel {
public:
    MenuModel();
    void nextOption();
    void previousOption();


    void setSelectedIndex(int index);
    int getSelectedIndex() const;

    bool isGameStarted() const;
    void startGame();

    MenuState getState() const {return m_currentState;}
    void setState(MenuState state) { m_currentState = state; }

private:
    int m_selectedIndex;
    int m_maxOptions;
    bool m_isGameStarted;
    MenuState m_currentState;
};

#endif
