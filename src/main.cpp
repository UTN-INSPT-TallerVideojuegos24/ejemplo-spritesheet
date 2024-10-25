#include "sfml-utn-inspt.h"

// Parámetros de la ventana:
const unsigned int ANCHO_VENT = 400;
const unsigned int ALTO_VENT = 300;
const unsigned int FRAMERATE = 120;

// Utilizado para moverse a la fila correspondiente del spritesheet:
enum mov_t { UP, LEFT, DOWN, RIGHT };
// Cantidad máxima de animaciones (fila) del spritesheet:
const unsigned int MAX_MOV = 9;
// Tamaño del sprite individual:
const Vector2 SPR_SIZE = {64, 64};

// El movimiento y la animación dependen del FRAMERATE.
// Esto es importante, porque de no estar, la animación del personaje
// se haría a la misma velocidad del FRAMERATE, lo que daría un movimiento
// demasiado rápido (por lo general).
const int MOVE_CYCLE_TIME = FRAMERATE / 16;
const int VELOCIDAD = 4.5f;

// lee los eventos (clicks, posición del mouse, redimensión de la ventana, etc)
void leer_eventos(RenderWindow &window);

// Tipo de dato custom, borde de la ventana, para detectar coliciones
enum borde_t { SUPERIOR, INFERIOR, IZQUIERDO, DERECHO };

/**
 * @brief Detecta la colición de un rectangulo con la ventana
 *
 * @param r Rectangulo a evaluar si coliciona
 * @param borde Con qué borde (borde_t) tiene que contrastar la colisión
 * @return true si ha colisionado el rectangulo `r` con el `borde` especificado
 * @return false si no ha colicionado o se pasa un `borde` inválido.
 */
bool colision_con_ventana(const FloatRect &t, borde_t borde);

int main() {
  RenderWindow ventana({ANCHO_VENT, ALTO_VENT}, "UTN-INSPT Spritesheet");
  ventana.setFramerateLimit(FRAMERATE);
  ventana.setMouseCursorVisible(false);
  // Spritesheet con todos los movimientos:
  Texture punga_texture;
  if (!punga_texture.loadFromFile("../assets/punga.png")) {
    cerr << "No se pudo cargar la textura del personaje." << endl;
    exit(-1);
  }
  // movimientos del sprite (coordenadas del spritesheet)
  Vector2i spr_mov = {0, DOWN}; //  Inicialmente parado y mirando para abajo
  Sprite punga(punga_texture);
  // Se inicializa la variable para llevar adelante la animación y el
  // movimiento.
  int mov_cycle_time = MOVE_CYCLE_TIME;
  while (ventana.isOpen()) {
    // Se leen los eventos de la ventana:
    leer_eventos(ventana);

    mov_cycle_time--;
    if (mov_cycle_time == 0) {
      // Una vez cada MOVE_CYCLE_TIME veces:
      mov_cycle_time = MOVE_CYCLE_TIME;
      // Flag de tecla precioanda
      bool key_pressed = false;
      if (Keyboard::isKeyPressed(Keyboard::S)) {
        key_pressed = true;
        spr_mov.y = DOWN;
        if (!colision_con_ventana(punga.getGlobalBounds(), INFERIOR)) {
          punga.move(0, VELOCIDAD);
        }
      } else if (Keyboard::isKeyPressed(Keyboard::W)) {
        key_pressed = true;
        spr_mov.y = UP;
        if (!colision_con_ventana(punga.getGlobalBounds(), SUPERIOR)) {
          punga.move(0, -VELOCIDAD);
        }
      }
      if (Keyboard::isKeyPressed(Keyboard::A)) {
        key_pressed = true;
        spr_mov.y = LEFT;
        if (!colision_con_ventana(punga.getGlobalBounds(), IZQUIERDO)) {
          punga.move(-VELOCIDAD, 0);
        }
      } else if (Keyboard::isKeyPressed(Keyboard::D)) {
        key_pressed = true;
        spr_mov.y = RIGHT;
        if (!colision_con_ventana(punga.getGlobalBounds(), DERECHO)) {
          punga.move(VELOCIDAD, 0);
        }
      }
      // Si una tecla fue precionada, hay que mover el sprite del personaje:
      if (key_pressed) {
        spr_mov.x++; // Se incrementa en 1 para moverse un sprite dentro del
                     // spritesheet
        if (spr_mov.x == MAX_MOV) { // Si se alcanzó el límite de la animación
          spr_mov.x = 1;            // Se ciclea con el movimiento
        }
      } else {
        // Si no se precionó ninguna tecla se vuelve a la primer columna
        // (personaje parado)
        spr_mov.x = 0;
      }
    }
    // Se toma el sprite adecuado del spritesheet para dibujar:
    punga.setTextureRect(IntRect(spr_mov.x * SPR_SIZE.x, spr_mov.y * SPR_SIZE.y,
                                 SPR_SIZE.x, SPR_SIZE.y));

    // Se dibuja el personaje en la pantalla:
    ventana.clear();
    ventana.draw(punga);
    ventana.display();
  }
}

void leer_eventos(RenderWindow &window) {
  // Se "leen" los eventos y se guardan en event.
  Event event = Event();
  // Se recorren los eventos hasta que ya no hayan más
  while (window.pollEvent(event)) {
    if (event.type == Event::Closed) {
      // Si el usuario tocó la [X] para cerra la ventana:
      window.close();
    }
  }
}

bool colision_con_ventana(const FloatRect &fr, borde_t borde) {
  switch (borde) {
  case SUPERIOR:
    return fr.top <= 0;
  case INFERIOR:
    return (fr.top + fr.height) >= ALTO_VENT;
  case IZQUIERDO:
    return fr.left <= 0;
  case DERECHO:
    return (fr.left + fr.width) >= ANCHO_VENT;
  }
  return false;
}
