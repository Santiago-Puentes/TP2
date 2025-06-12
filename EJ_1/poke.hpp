#pragma once
#include <string> //std::string
#include <vector> //std::vector
#include <array> //std::array
#include <utility> //std::pair
#include <unordered_map> //std::unordered_map
#include <iostream>
#include <functional>
#include <fstream>

using namespace std;

using uint = unsigned int;
using set_ataques = vector<pair<string, uint>>;

class Pokemon {
    string nombre;
    uint experiencia;
    public:
        Pokemon() = default;
        Pokemon(const string& name, uint xp) : nombre(name), experiencia(xp) {};
        const string& getNombre() const {return nombre;}
        uint getXP() const {return experiencia;}
        bool operator==(const Pokemon&) const;
        ~Pokemon() = default;
};

class PokemonInfo {
    string tipo;
    string descripcion;
    set_ataques ataquesPNivel;
    array<uint, 3> experienciaPNivel;
    public:
        PokemonInfo() = default;
        PokemonInfo(const string& t, const string& d, const set_ataques& a, const array<uint, 3>& xp) :
                tipo(t), descripcion(d), ataquesPNivel(a), experienciaPNivel(xp) {}
        const string& getTipo() const {return tipo;}
        const string& getDescripcion() const {return descripcion;}
        const set_ataques& getAtaques_lvl() const {return ataquesPNivel;}
        const array<uint, 3>& getXP_lvl() const {return experienciaPNivel;}
        ~PokemonInfo() = default;
};

// Declaraciones de operadores de salida
ostream& operator<<(ostream& os, const Pokemon& poke);
ostream& operator<<(ostream& os, const PokemonInfo& info);

namespace std {
    template<>
    struct hash<Pokemon>{
        size_t operator()(const Pokemon& p) const {
            return hash<string>()(p.getNombre());
        }
    };
}

class Pokedex {
    unordered_map<Pokemon, PokemonInfo> Pokedata;
    string saveFile;

    public:
        Pokedex() = default;
        Pokedex(string save_file) : saveFile(save_file) {deserializar();}

        void mostrar(const Pokemon& poke) const;
        void mostrarTodos() const;
        void serializar() const;
        void deserializar();
        ~Pokedex() {
            serializar();
        }
        PokemonInfo& operator[](const Pokemon& poke) {return Pokedata[poke];}
};