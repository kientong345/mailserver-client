#ifndef __CLIENT_GRAPHIC_H__
#define __CLIENT_GRAPHIC_H__

class ClientGraphic {
private:
    uint8_t _cur_page;

    ClientGraphic();

public:
    static ClientGraphic* getInstance();

    void displayRegistation();
    void displayMenu();

    ~ClientGraphic();

};

#endif /* __CLIENT_GRAPHIC_H__ */