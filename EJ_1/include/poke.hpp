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
using set_ataques = array<pair<string, uint>, 3>;

class Pokemon {
    string nombre;
    uint experiencia;
    public:
        Pokemon(const string&, uint);
        const string& getNombre() const;
        uint getXP() const;
        void setXP(uint);
        ~Pokemon() = default;
        bool operator==(const Pokemon&);
    friend ostream& operator<<(ostream&, const Pokemon&);
};

namespace std {
    template<>
    struct hash<const Pokemon&>{
        size_t operator()(const Pokemon& p) const {
            return hash<string>()(p.getNombre());
        }
    };
}

class PokemonInfo {
    string tipo;
    string descripcion;
    set_ataques ataquesPNivel;
    array<uint, 3> experienciaPNivel;
    public:
        PokemonInfo(const string&, const string&, const set_ataques &, const array<uint, 3>&);
        const string& getTipo() const;
        const string& getDescripcion() const;
        const set_ataques& getAtaquesPNivel() const;
        const array<uint, 3>& getXP_lvl() const;
        ~PokemonInfo() = default;
    friend ostream& operator<<(ostream&, const PokemonInfo&);
};

class Pokedex {
    unordered_map<Pokemon, PokemonInfo> Pokedata;
    public:
        Pokedex(const unordered_map<Pokemon, PokemonInfo>&);
        Pokedex(const unordered_map<Pokemon, PokemonInfo>&, const string&);
        void mostrar(const Pokemon& p) const;
        void mostrarTodos() const;
        ~Pokedex() = default;
        void serializar(ofstream& out) const;
        void deserializar(ifstream& in);
};
