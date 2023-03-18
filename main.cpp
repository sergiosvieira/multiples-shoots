#include <iostream>
#include <vector>
#include "raylib.h"

using namespace std;


class Movimento {
protected:
    int x = 0, y = 0;
    float velocidade = 10.f;
    float vx = 0.f, vy = 0.f;
public:
    Movimento(int x, int y): x(x), y(y){}
    virtual void mover() {
        x += vx;
        y += vy;
    }
    virtual void desenhar() = 0;
    void setX(int x) {
        this->x = x;
    }
    void setY(int y) {
        this->y = y;
    }
    int getX() const {
        return this->x;
    }
    int getY() const {
        return this->y;
    }
    void moverEsquerda() {
        vx = -velocidade;
    }
    void moverDireita() {
        vx = velocidade;
    }
    void moverCima() {
        vy = -velocidade;
    }
    void moverBaixo() {
        vy = velocidade;
    }
    void parar() {
        vx = 0.f;
        vy = 0.f;
    }
};

class Tiro : public Movimento {
    bool visivel = false;
    int largura = 0, altura = 0;
public:
    Tiro(int x, int y, int largura, int altura):
        Movimento(x, y), largura(largura), altura(altura){}
    void mover() override {
        Movimento::mover();
        if (getY() < -altura) {
            tornarInvisivel();
        }
    }
    void desenhar() override {
        DrawRectangle(x, y, largura, altura, RED);
    }
    void tornarVisivel() {
        visivel = true;
    }
    void tornarInvisivel() {
        visivel = false;
    }
    bool estaVisivel() {
        return visivel;
    }
    int getLargura() const {
        return largura;
    }
    int getAltura() const {
        return altura;
    }
};

class Nave: public Movimento {
    int largura = 0, altura = 0;
    std::vector<Tiro> tiros;
public:
    Nave(int x, int y, int largura, int altura):
        Movimento(x, y), largura(largura), altura(altura){
        for (size_t i = 0; i < 20; ++i) {
            Tiro t = Tiro(0, 0, 5, 10);
            t.moverCima();
            tiros.push_back(t);
        }
    }
    void mover() override {
        parar();
        if (IsKeyDown(KEY_LEFT)) {
            moverEsquerda();
        } else if (IsKeyDown(KEY_RIGHT)) {
            moverDireita();
        }
        Movimento::mover();
    }
    void desenhar() override {
        Vector2 v = {x + largura / 2.f, static_cast<float>(y)};
        DrawCircleSector(v, 8., 0., 270., 320., WHITE);
        DrawRectangle(x, y, largura, altura, WHITE);
        v = {x + largura / 2.f, static_cast<float>(y + altura)};
        DrawCircleSector(v, 25., 270., 90., 90., WHITE);
        for (Tiro& t: tiros) {
            if (t.estaVisivel()) t.desenhar();
        }
    }
    void atirar() {
        for (Tiro& t: tiros) {
            if (!t.estaVisivel()) {
                t.setX(x + (largura - t.getLargura()) / 2);
                t.setY(y + t.getAltura());
                t.tornarVisivel();
                break;
            }
        }
    }
    void mover_tiros() {
        for (Tiro& t: tiros) {
            if (t.estaVisivel()) {
                t.mover();
            }
        }
    }
};

int main()
{
    int screenWidth = 1024;
    int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "");
    SetTargetFPS(60);
    Nave nave(screenWidth/2, screenHeight - 60, 15, 50);
    float timer = 0.f;
    while (!WindowShouldClose()) {
        timer += GetFrameTime();
        nave.mover();
        if (IsKeyDown(KEY_SPACE)
                && timer >= 0.1f) {
            nave.atirar();
            timer = 0.0f;
        }
        nave.mover_tiros();
        BeginDrawing();
        ClearBackground(BLACK);
        nave.desenhar();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
