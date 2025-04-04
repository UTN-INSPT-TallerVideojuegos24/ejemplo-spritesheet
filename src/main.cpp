#include "sfml-utn-inspt.h"

// Parámetros de la ventana:
const unsigned int ANCHO_VENT = 400;
const unsigned int ALTO_VENT = 300;
const unsigned int FRAMERATE = 60;

// Path donde se encuentran los assets (sprites, texturas, etc)
const string ASSETS_PATH = "../assets/";
// Nombre del spritesheet del personaje:
const string PUNGA_SPRITESHEET = "punga.png";

// Utilizado para moverse a la fila correspondiente del spritesheet:
enum mov_t { ARRIBA, IZQ, ABAJO, DER };
// Cantidad máxima de animaciones (columnas) del spritesheet:
const unsigned int MAX_MOV = 9;
// Tamaño del sprite individual:
const float SPR_SIZE = 64.f;

// El movimiento y la animación dependen del FRAMERATE.
// Esto es importante, porque de no estar, la animación del personaje
// se haría a la misma velocidad del FRAMERATE, lo que daría un movimiento
// demasiado rápido (por lo general).
const int MOVE_CYCLE_TIME = FRAMERATE / 16;
const float VELOCIDAD = 4.5f;

// lee los eventos (clicks, posición del mouse, redimensión de la ventana, etc)
void leer_eventos(RenderWindow &ventana);

// Tipo de dato custom, borde de la ventana, para detectar coliciones
enum borde_t { SUPERIOR, INFERIOR, IZQUIERDO, DERECHO };

/**
 * @brief Detecta la colición de un rectangulo con la ventana
 *
 * @param fr Rectangulo a evaluar si coliciona
 * @param borde Con qué borde (borde_t) tiene que contrastar la colisión
 * @return true si ha colisionado el rectangulo `fr` con el `borde` especificado
 * @return false si no ha colicionado o se pasa un `borde` inválido.
 */
bool colision_con_ventana(const FloatRect &fr, borde_t borde);

int main() {
  RenderWindow ventana({ANCHO_VENT, ALTO_VENT}, "UTN-INSPT Spritesheet");
  ventana.setFramerateLimit(FRAMERATE);
  ventana.setMouseCursorVisible(false);
  // Spritesheet con todos los movimientos:
  Texture punga_texture;
  if (!punga_texture.loadFromFile(ASSETS_PATH + PUNGA_SPRITESHEET)) {
    cerr << "No se pudo cargar la textura del personaje." << endl;
    exit(-1);
  }
  // movimientos del sprite (coordenadas del spritesheet)
  Vector2i spr_mov = {0, ABAJO}; //  Inicialmente parado y mirando para abajo
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
      bool esta_moviendose = false;
      Vector2f desplazamiento = {0.f, 0.f};
      if (Keyboard::isKeyPressed(Keyboard::S)) {
        esta_moviendose = true;
        spr_mov.y = ABAJO;
        if (!colision_con_ventana(punga.getGlobalBounds(), INFERIOR)) {
          desplazamiento.y = 1;
        }
      } else if (Keyboard::isKeyPressed(Keyboard::W)) {
        esta_moviendose = true;
        spr_mov.y = ARRIBA;
        if (!colision_con_ventana(punga.getGlobalBounds(), SUPERIOR)) {
          desplazamiento.y = -1;
        }
      }
      if (Keyboard::isKeyPressed(Keyboard::A)) {
        esta_moviendose = true;
        spr_mov.y = IZQ;
        if (!colision_con_ventana(punga.getGlobalBounds(), IZQUIERDO)) {
          desplazamiento.x = -1;
        }
      } else if (Keyboard::isKeyPressed(Keyboard::D)) {
        esta_moviendose = true;
        spr_mov.y = DER;
        if (!colision_con_ventana(punga.getGlobalBounds(), DERECHO)) {
          desplazamiento.x = 1;
        }
      }
      // Si una tecla fue precionada, hay que mover el sprite del personaje:
      if (esta_moviendose) {
        punga.move(desplazamiento * VELOCIDAD);
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
    punga.setTextureRect(IntRect(spr_mov.x * SPR_SIZE, spr_mov.y * SPR_SIZE,
                                 SPR_SIZE, SPR_SIZE));

    // Se dibuja el personaje en la pantalla:
    ventana.clear();
    ventana.draw(punga);
    ventana.display();
  }
}

void leer_eventos(RenderWindow &ventana) {
  // Se "leen" los eventos y se guardan en event.
  Event event = Event();
  // Se recorren los eventos hasta que ya no hayan más
  while (ventana.pollEvent(event)) {
    if (event.type == Event::Closed) {
      // Si el usuario tocó la [X] para cerra la ventana:
      ventana.close();
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
