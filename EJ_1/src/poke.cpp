#include "poke.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <iterator>
#include <concepts>

template<typename X>
concept Printable = requires(ostream& os, const X& x) {
    { os << x } -> same_as<ostream&>;
};

Pokemon::Pokemon(const string& name, uint xp) : nombre(name), experiencia(xp) {};

const string& Pokemon::getNombre() const {return nombre;}

uint Pokemon::getXP() const {return experiencia;}

void Pokemon::setXP(uint xp) {experiencia = xp;}

PokemonInfo::PokemonInfo(const string& t, const string& d, const set_ataques& a, const array<uint, 3>& xp) :
        tipo(t), descripcion(d), ataquesPNivel(a), experienciaPNivel(xp) {}

const string& PokemonInfo::getTipo() const {return tipo;}

const string& PokemonInfo::getDescripcion() const {return descripcion;}

const set_ataques& PokemonInfo::getAtaquesPNivel() const {return ataquesPNivel;}

const array<uint, 3>& PokemonInfo::getXP_lvl() const {return experienciaPNivel;}

Pokedex::Pokedex(const unordered_map<Pokemon, PokemonInfo>& data) : Pokedata(data){}
Pokedex::Pokedex(const unordered_map<Pokemon, PokemonInfo>& data, const string& file) : Pokedata(data) {
    //revisar aca
}

void Pokedex::mostrar(const Pokemon& p) const {
    try {
        const auto& search = Pokedata.at(p);
        cout << "Pokemon encontrado...\n" << p << "\n" << search << endl;
    }
    catch (const out_of_range&) {cout << "¡Pokemon desconocido!" << endl;}
}

void Pokedex::mostrarTodos() const {
    for (const auto& to_print:Pokedata) {cout << to_print;}
}

bool Pokemon::operator==(const Pokemon& other) {
    return (nombre == other.nombre && experiencia == other.experiencia);
}

ostream& operator<<(ostream& os, const Pokemon& poke) {
    os << "Nombre: " << poke.nombre << ", XP: " << poke.experiencia << endl;
    return os;
}

template<Printable T, Printable U>
std::ostream& operator<<(ostream& os, const pair<T, U>& p) {
    os << "(" << p.first << ": " << p.second << ")";
    return os;
}

ostream& operator<<(ostream& os, const PokemonInfo& info) {
    os << "Tipo: " << info.tipo
        << "\nDescripción: " << info.descripcion
        << "\nAtaques por nivel: ";
        copy(info.ataquesPNivel.begin(), info.ataquesPNivel.end(), ostream_iterator<int>(os, "-"));
    os << "Experiencia por nivel: ";
        copy(info.experienciaPNivel.begin(), info.experienciaPNivel.end(), ostream_iterator<int>(os, "-"));
    os << endl;
    return os;
}

