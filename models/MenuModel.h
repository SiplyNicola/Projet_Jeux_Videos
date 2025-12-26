#ifndef MENUMODEL_H
#define MENUMODEL_H

class MenuModel {
public:
    MenuModel();
    void nextOption();
    void previousOption();


    void setSelectedIndex(int index);
    int getSelectedIndex() const;

    bool isGameStarted() const;
    void startGame();

private:
    int m_selectedIndex;
    int m_maxOptions;
    bool m_isGameStarted;
};

#endif
