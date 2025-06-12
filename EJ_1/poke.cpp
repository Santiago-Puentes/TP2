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

template<Printable T, Printable U>
ostream& operator<<(ostream& os, const pair<T, U>& p) {
    os << "(" << p.first << ": " << p.second << ")";
    return os;
}

bool Pokemon::operator==(const Pokemon& other) {
    return (nombre == other.nombre && experiencia == other.experiencia);
}

ostream& operator<<(ostream& os, const Pokemon& poke) {
    os << "Nombre: " << poke.getNombre() << ", XP: " << poke.getXP() << endl;
    return os;
}


ostream& operator<<(ostream& os, const PokemonInfo& info) {
    os << "Tipo: " << info.getTipo()
        << "\nDescripción: " << info.getDescripcion()
        << "\nAtaques disponiblees: ";
        copy(info.getAtaques_lvl().begin(), info.getAtaques_lvl().end(), ostream_iterator<int>(os, "-"));
    os << "Experiencia por nivel: ";
        copy(info.getXP_lvl().begin(), info.getXP_lvl().end(), ostream_iterator<int>(os, "-"));
    os << endl;
    return os;
}

void Pokedex::mostrar(const Pokemon& poke) const {
    try {
        const auto& search = Pokedata.at(poke);
        cout << "Pokemon encontrado...\n" << poke << "\n" << search << endl;
    }
    catch (const out_of_range&) {cout << "¡Pokemon desconocido!" << endl;}
}

void Pokedex::mostrarTodos() const {
    for (const auto& to_print:Pokedata) {cout << to_print;}
}

void Pokedex::serializar() const {
    ofstream out(saveFile, ios::binary);
    if (!out) {
        cerr << "Error al abrir el archivo de guardado para escritura.\n";
        return;
    }

    uint cant = Pokedata.size();
    out.write(reinterpret_cast<const char*>(&cant), sizeof(cant));

    for (const auto& [poke, info] : Pokedata) {
        // Serializar nombre
        uint len = poke.getNombre().size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(poke.getNombre().c_str(), len);

        // Serializar experiencia
        uint xp = poke.getXP();
        out.write(reinterpret_cast<const char*>(&xp), sizeof(xp));

        // Serializar tipo y descripción
        auto write_string = [&](const string& s) {
            uint len = s.size();
            out.write(reinterpret_cast<const char*>(&len), sizeof(len));
            out.write(s.c_str(), len);
        };

        write_string(info.getTipo());
        write_string(info.getDescripcion());

        // Serializar ataques por nivel
        const auto& ataques = info.getAtaques_lvl();
        uint cant_ataques = ataques.size();
        out.write(reinterpret_cast<const char*>(&cant_ataques), sizeof(cant_ataques));
        for (const auto& [nombre, dmg] : ataques) {
            write_string(nombre);
            out.write(reinterpret_cast<const char*>(&dmg), sizeof(dmg));
        }

        // Serializar experiencia por nivel
        const auto& xp_niveles = info.getXP_lvl();
        for (uint xp_n : xp_niveles) {
            out.write(reinterpret_cast<const char*>(&xp_n), sizeof(xp_n));
        }
    }

    // el destructor de ofstream lo cierra solo
}

void Pokedex::deserializar() {
    ifstream in(saveFile, ios::binary);
    if (!in) {
        cerr << "No se pudo abrir el archivo de guardado para lectura.\n";
        return;
    }

    uint cant;
    in.read(reinterpret_cast<char*>(&cant), sizeof(cant));

    for (uint i = 0; i < cant; ++i) {
        auto read_string = [&](string& s) {
            uint len;
            in.read(reinterpret_cast<char*>(&len), sizeof(len));
            s.resize(len);
            in.read(&s[0], len);
        };

        string nombre;
        uint xp;
        read_string(nombre);
        in.read(reinterpret_cast<char*>(&xp), sizeof(xp));
        Pokemon poke(nombre, xp);

        string tipo, descripcion;
        read_string(tipo);
        read_string(descripcion);

        uint cant_ataques;
        in.read(reinterpret_cast<char*>(&cant_ataques), sizeof(cant_ataques));
        set_ataques ataques;
        for (uint j = 0; j < cant_ataques; ++j) {
            string nombre_ataque;
            uint dmg;
            read_string(nombre_ataque);
            in.read(reinterpret_cast<char*>(&dmg), sizeof(dmg));
            ataques.emplace_back(nombre_ataque, dmg);
        }

        array<uint, 3> xp_niveles;
        for (uint& xp_n : xp_niveles) {
            in.read(reinterpret_cast<char*>(&xp_n), sizeof(xp_n));
        }

        PokemonInfo info(tipo, descripcion, ataques, xp_niveles);
        Pokedata.insert({poke, info});
    }
}
