#pragma once

#include <string>           //std::string
#include <vector>           //std::vector
#include <array>            //std::array
#include <utility>          //std::pair
#include <unordered_map>    //std::unordered_map
#include <iostream>         //std::cout
#include <fstream>          //std::ofstream y std::ifstream

using namespace std;

// Nomenclaturas
using uint = unsigned int;  
using set_ataques = vector<pair<string, uint>>;

// Clase de Pokemon
class Pokemon {
    string nombre;
    uint experiencia;
    public:
        Pokemon() = default;    // Para estructuras que inicialicen el objeto default
        // Constructor general
        Pokemon(const string& name, uint xp) : nombre(name), experiencia(xp) {};
        // Devuelve el nombre del Pokemon
        const string& getNombre() const {return nombre;}
        // Devuelve el xp del Pokemon
        uint getXP() const {return experiencia;}
        // Permite la comparación entre dos objetos Pokemon mediante el nombre
        bool operator==(const Pokemon& other) const {return (nombre == other.nombre);}
        ~Pokemon() = default;   // Destrucción default
};

// Clase de PokemonInfo
class PokemonInfo {
    string tipo;                        // Tipo del pokemon
    string descripcion;                 // Descripción del pokemon
    set_ataques ataquesPNivel;          // Ataques del pokemon
    array<uint, 3> experienciaPNivel;   // Experiencia para llegar a cada nivel
    public:
        PokemonInfo() = default;     // Para estructuras que inicialicen el objeto default
        // Constructor general de la clase
        PokemonInfo(const string& t, const string& d, const set_ataques& a, const array<uint, 3>& xp) :
                tipo(t), descripcion(d), ataquesPNivel(a), experienciaPNivel(xp) {}
        // Devuelve el tipo del Pokemon
        const string& getTipo() const {return tipo;}
        // Devuelve la descripcion del Pokemon
        const string& getDescripcion() const {return descripcion;}
        // Devuelve los ataques del Pokemon
        const set_ataques& getAtaques_lvl() const {return ataquesPNivel;}
        // Devuelve la experiencia de cada nivel del Pokemon
        const array<uint, 3>& getXP_lvl() const {return experienciaPNivel;}
        ~PokemonInfo() = default;   // Destrucción default
};

// Declaraciones de operadores de salida
ostream& operator<<(ostream& os, const Pokemon& poke);
ostream& operator<<(ostream& os, const PokemonInfo& info);

// Especialización para Pokemon del functor de hash utilizado por std::unordered_map 
namespace std {
    template<>
    struct hash<Pokemon>{
        size_t operator()(const Pokemon& p) const {
            return hash<string>()(p.getNombre());   // Se define el functor como el retorno de un hash del nombre del pokemon
        }
    };
}

// Clase de Pokedex
class Pokedex {
    unordered_map<Pokemon, PokemonInfo> Pokedata;   // Dex con datos de pokemones descubiertos
    string saveFile;        // Nombre del archivo para serialización

    public:
        Pokedex() = default;        // Para inicialización default

        /*
        * Constructor específico para iniciar la pokedex en base a un archivo binario.
        * Se deserializa una vez inicializada la estructura
        */ 
        Pokedex(string save_file) : saveFile(save_file) {deserializar();}
        // Muestra por pantalla un objeto Pokemon si se encuentra en la pokedata
        void mostrar(const Pokemon& poke) const;
        // Muestra todos los pokemones contenidos por pokedata
        void mostrarTodos() const;
        // Permite guardar toda la informació de la pokedex en un archivo de salida binario
        void serializar() const;
        // Permite cargar toda la información de una versión de una pokedex almacenada en un archivo de entrada binario
        void deserializar();
        // Destructor específico para serializar si hace falta
        ~Pokedex() {
            if(!saveFile.empty()) serializar(); // Si tiene un file para serializar entonces se serializa antes de destruir la instancia
        }
        // Operador para facilitar la búsqueda e incersión en el mapa
        PokemonInfo& operator[](const Pokemon& poke) {return Pokedata[poke];}
};