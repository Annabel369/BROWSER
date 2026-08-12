#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// =======================================================
// Tipos de Elementos Suportados
// =======================================================
enum ElementType {
    TAG_TEXT,
    TAG_H1,
    TAG_P,
    TAG_A,
    TAG_BR
};

// =======================================================
// Estrutura do Nó do DOM (Memória Leve)
// =======================================================
struct DOMNode {
    ElementType type;
    char text[128];
    char href[128];
    
    int x, y;
    int width, height;
    
    DOMNode* next;
};

// =======================================================
// O "Motor" de Layout e Renderização
// =======================================================
class LayoutEngine {
private:
    int cursorX;
    int cursorY;
    int screenWidth;
    
public:
    LayoutEngine(int sWidth) {
        screenWidth = sWidth;
        reset();
    }
    
    void reset() {
        cursorX = 10;
        cursorY = 20;
    }

    void renderTree(DOMNode* head) {
        DOMNode* current = head;
        reset();
        
        while (current != NULL) {
            switch (current->type) {
                case TAG_H1:
                    cursorY += 15;
                    setcolor(YELLOW);
                    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
                    
                    current->x = cursorX;
                    current->y = cursorY;
                    current->width = textwidth(current->text);
                    current->height = textheight(current->text);
                    
                    outtextxy(cursorX, cursorY, current->text);
                    cursorY += current->height + 15;
                    cursorX = 10;
                    break;
                    
                case TAG_P:
                case TAG_TEXT:
                    setcolor(WHITE);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                    
                    if (cursorX + textwidth(current->text) > screenWidth - 10) {
                        cursorX = 10;
                        cursorY += textheight(current->text) + 5;
                    }
                    
                    current->x = cursorX;
                    current->y = cursorY;
                    current->width = textwidth(current->text);
                    current->height = textheight(current->text);
                    
                    outtextxy(cursorX, cursorY, current->text);
                    cursorX += current->width + 5;
                    break;
                    
                case TAG_A:
                    setcolor(LIGHTBLUE);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                    
                    current->x = cursorX;
                    current->y = cursorY;
                    current->width = textwidth(current->text);
                    current->height = textheight(current->text);
                    
                    outtextxy(cursorX, cursorY, current->text);
                    line(cursorX, cursorY + current->height + 2, 
                         cursorX + current->width, cursorY + current->height + 2);
                         
                    cursorX += current->width + 5;
                    break;
                    
                case TAG_BR:
                    cursorX = 10;
                    cursorY += 15;
                    break;
            }
            current = current->next;
        }
    }
    
    char* checkClick(DOMNode* head, int mouseX, int mouseY) {
        DOMNode* current = head;
        while (current != NULL) {
            if (current->type == TAG_A) {
                if (mouseX >= current->x && mouseX <= (current->x + current->width) &&
                    mouseY >= current->y && mouseY <= (current->y + current->height)) {
                    return current->href;
                }
            }
            current = current->next;
        }
        return NULL;
    }
};

// =======================================================
// Parser Simples (Fábrica de Nós)
// =======================================================
DOMNode* createNode(ElementType type, const char* text, const char* href = "") {
    DOMNode* node = new DOMNode;
    node->type = type;
    strncpy(node->text, text, 127);
    strncpy(node->href, href, 127);
    node->next = NULL;
    return node;
}

DOMNode* parseHTML(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        DOMNode* errorNode = createNode(TAG_H1, "Erro 404: Arquivo nao encontrado!");
        errorNode->next = createNode(TAG_P, filename);
        errorNode->next->next = createNode(TAG_BR, "");
        errorNode->next->next->next = createNode(TAG_A, "Sair", "exit");
        return errorNode;
    }
    
    DOMNode* head = NULL;
    DOMNode* tail = NULL;
    
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char* p = line;
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
        if (*p == '\0') continue;
        
        ElementType type = TAG_TEXT;
        char text[128] = {0};
        char href[128] = {0};
        
        if (strncmp(p, "<h1>", 4) == 0 || strncmp(p, "<H1>", 4) == 0) {
            type = TAG_H1;
            char* end = strstr(p + 4, "</h1>");
            if (!end) end = strstr(p + 4, "</H1>");
            if (end) {
                strncpy(text, p + 4, end - (p + 4));
            } else {
                strcpy(text, p + 4);
            }
        } else if (strncmp(p, "<p>", 3) == 0 || strncmp(p, "<P>", 3) == 0) {
            type = TAG_P;
            char* end = strstr(p + 3, "</p>");
            if (!end) end = strstr(p + 3, "</P>");
            if (end) {
                strncpy(text, p + 3, end - (p + 3));
            } else {
                strcpy(text, p + 3);
            }
        } else if (strncmp(p, "<br>", 4) == 0 || strncmp(p, "<BR>", 4) == 0) {
            type = TAG_BR;
        } else if (strncmp(p, "<a href=\"", 9) == 0 || strncmp(p, "<A HREF=\"", 9) == 0) {
            type = TAG_A;
            char* hrefStart = p + 9;
            char* hrefEnd = strchr(hrefStart, '\"');
            if (hrefEnd) {
                strncpy(href, hrefStart, hrefEnd - hrefStart);
                char* textStart = strchr(hrefEnd, '>');
                if (textStart) {
                    textStart++;
                    char* textEnd = strstr(textStart, "</a>");
                    if (!textEnd) textEnd = strstr(textStart, "</A>");
                    if (textEnd) {
                        strncpy(text, textStart, textEnd - textStart);
                    } else {
                        strcpy(text, textStart);
                    }
                }
            }
        } else {
            type = TAG_TEXT;
            strcpy(text, p);
        }
        
        char* nl = strchr(text, '\n');
        if (nl) *nl = '\0';
        nl = strchr(text, '\r');
        if (nl) *nl = '\0';
        
        DOMNode* node = createNode(type, text, href);
        if (!head) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(fp);
    if (!head) head = createNode(TAG_H1, "Pagina Vazia");
    return head;
}

// =======================================================
// Funções de Interrupção de Mouse (INT 33h)
// =======================================================
void initMouse(int &exists) {
    union REGS regs;
    regs.x.ax = 0x00;
    int86(0x33, &regs, &regs);
    exists = regs.x.ax;
}

void showMouse() {
    union REGS regs;
    regs.x.ax = 0x01;
    int86(0x33, &regs, &regs);
}

void hideMouse() {
    union REGS regs;
    regs.x.ax = 0x02;
    int86(0x33, &regs, &regs);
}

void getMousePos(int &x, int &y, int &button) {
    union REGS regs;
    regs.x.ax = 0x03;
    int86(0x33, &regs, &regs);
    button = regs.x.bx;
    x = regs.x.cx;
    y = regs.x.dx;
}

// =======================================================
// Loop Principal da Aplicação
// =======================================================
int main() {
    int gd = VGA, gm = VGAHI, err;
    int mouseExists, mouseX, mouseY, mouseButton;
    int isPressed = 0;
    
    // Inicializa graficos
    initgraph(&gd, &gm, "C:\\BCPP31\\BGI"); 
    
    err = graphresult();
    if (err != grOk) {
        printf("Erro inicializando graficos: %s\n", grapherrormsg(err));
        return 1;
    }
    
    initMouse(mouseExists);
    if (!mouseExists) {
        closegraph();
        printf("Mouse driver não encontrado!\n");
        return 1;
    }
    
    // Configura UI Inicial
    setbkcolor(BLUE);
    cleardevice();
    
    // Carrega a pagina inicial HTML
    char currentUrl[128] = "PAGE1.HTM";
    DOMNode* page = parseHTML(currentUrl);
    
    // Renderiza
    LayoutEngine engine(640);
    engine.renderTree(page);
    
    showMouse();
    
    // Event Loop
    int running = 1;
    while (running && !kbhit()) {
        getMousePos(mouseX, mouseY, mouseButton);
        
        if (mouseButton & 1) { // Clique esquerdo
            if (!isPressed) {
                isPressed = 1;
                char* clickedUrl = engine.checkClick(page, mouseX, mouseY);
                if (clickedUrl != NULL) {
                    if (strcmp(clickedUrl, "exit") == 0) {
                        running = 0; // Finaliza aplicação
                    } else {
                        hideMouse();
                        setcolor(LIGHTGREEN);
                        outtextxy(10, 400, "Navegando para: ");
                        outtextxy(150, 400, clickedUrl);
                        showMouse();
                        delay(1000);
                        
                        hideMouse();
                        
                        // Libera memoria atual
                        DOMNode* current = page;
                        while(current != NULL) {
                            DOMNode* temp = current;
                            current = current->next;
                            delete temp;
                        }
                        
                        // Carrega nova pagina
                        strncpy(currentUrl, clickedUrl, 127);
                        page = parseHTML(currentUrl);
                        
                        // Limpa tela e re-renderiza
                        cleardevice();
                        engine.renderTree(page);
                        
                        showMouse();
                    }
                }
            }
        } else {
            isPressed = 0;
        }
    }
    
    // Limpeza
    DOMNode* current = page;
    while(current != NULL) {
        DOMNode* temp = current;
        current = current->next;
        delete temp;
    }
    
    hideMouse();
    closegraph();
    return 0;
}
