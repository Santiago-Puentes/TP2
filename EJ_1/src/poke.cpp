#include "poke.hpp"

ostream& operator<<(ostream& os, const Pokemon& poke) {
    os << "Nombre: " << poke.getNombre()        // Manda al stream el nombre
        << "\nXP: " << poke.getXP() << endl;    // Manda al stream el xp
    return os;  // Devuelve el stream
}

ostream& operator<<(ostream& os, const PokemonInfo& info) {
    os << "Tipo: " << info.getTipo()    // Manda al stream el tipo
        << "\nDescripción: " << info.getDescripcion() // Manda al stream la descripcion
        << "\nAtaques disponibles: [";
    for (const auto& [ataque, daño] : info.getAtaques_lvl()) {  
        os << "(" << ataque << ": " << daño << ")"; // Manda al stream los ataques pareados con cada nivel de daño
    }
    os << "]\nExperiencia por nivel: ";
    os << "[";
    for (const auto& xp : info.getXP_lvl()) {
        os << "(" << xp << ")"; // Manda al stream el xp requerida para cada nivel
    }
    os << "]" << endl;
    return os;  // Devuelve el stream
}

void Pokedex::mostrar(const Pokemon& poke) const {
    try {
        const auto& search = Pokedata.at(poke); // at() falla si no lo encuentra
        cout << "Pokemon encontrado...\n" << poke << "\n" << search << endl;    // Si tiene exito se manda al cout
    }
    catch (const out_of_range&) {cout << "¡Pokemon desconocido!" << endl;}  // Si no lo encuentra se imprime que se desconoce
}

void Pokedex::mostrarTodos() const {
    for (const auto& [poke, info]:Pokedata) {cout << poke << info << "\n";}
    // Iterando sobre cada pair del mapa se manda al cout por desempaquetado de la estructura del pair
}

void Pokedex::serializar() const {
    ofstream out(saveFile, ios::binary);    // Abre el archivo de nsalida
    if (!out) {
        // Si no se pudo abrir manda al stream de errores el aviso de error y no continua
        cerr << "Error al abrir el archivo: " << saveFile << "\n";
        return;
    }

    uint xp, ataques_size;  // Variables comunes necesitamos para la esscritura sobre el binario

    /**
     * Lambda de escritura para strings. Captura todo por referencia y recibe como parametro un string. 
     * Escribe sobre el file la longitud del string y el string en si casteado a const char * (como requiere el serialziado)
     */
    auto write_string = [&](const string& s) {
        uint str_len = static_cast<uint>(s.size());
        out.write(reinterpret_cast<const char*>(&str_len), sizeof(str_len));
        out.write(s.c_str(), str_len);
    };
    
    uint dex_size = static_cast<uint>(Pokedata.size()); // Vemos el tamaño de la pokedata
    out.write(reinterpret_cast<const char*>(&dex_size), sizeof(dex_size));  // Se escribe el tamaño de la pokedata en el file

    for (const auto& [poke, info] : Pokedata) { // Iteramos sobre cada pair {poke, info} serializando todos los datos
        write_string(poke.getNombre()); // Serializar nombre

        // Serializar experiencia
        xp = poke.getXP();
        out.write(reinterpret_cast<const char*>(&xp), sizeof(xp));

        write_string(info.getTipo()); // Serializar tipo
        write_string(info.getDescripcion()); // Serializar descripción

        // Serializar ataques por nivel
        const auto& ataques = info.getAtaques_lvl();
        ataques_size = static_cast<uint>(ataques.size());
        out.write(reinterpret_cast<const char*>(&ataques_size), sizeof(ataques_size));
        for (const auto& [nombre, dmg] : ataques) { // Iteramos sobre cada pair {nombre, dmg} serializando todos los datos
            write_string(nombre);   // Escribe el nombre como string
            out.write(reinterpret_cast<const char*>(&dmg), sizeof(dmg));
        }

        // Serializar experiencia por nivel
        for (uint xp_n : info.getXP_lvl()) {    // Iteramos sobre cada nivel para escribir toda la data
            out.write(reinterpret_cast<const char*>(&xp_n), sizeof(xp_n));
        }
    }
    out.close();    // Cierra el archiva
}

void Pokedex::deserializar() {
    ifstream in(saveFile, ios::binary);
    if (!in || in.peek() == ifstream::traits_type::eof()) {
        // Si no se pudo abrir manda al stream de errores el aviso de error y no continua
        cerr << "Error al abrir el archivo " << "\n";
        return;
    }

    // Inicializamos las variables temporales que se utilziarán para la lectura del file e incersión en la estructura
    uint dex_size, xp, ataques_size;
    string nombre, tipo, descripcion;
    set_ataques ataques;
    array<uint, 3> xp_niveles;

    // Lambda de lectura de string, análogo a write_string
    auto read_string = [&](string& s) {
        uint len;   // Para guardar el tamaño
        in.read(reinterpret_cast<char*>(&len), sizeof(len));    // Guarda el tamaño del string por leer
        s.resize(len);  // Reajusta el tamaño del string para evitar fuga de caracteres
        in.read(&s[0], len);    // Guarda el sttring leído en la referencia del parámetro especificando el largo
    };
    

    in.read(reinterpret_cast<char*>(&dex_size), sizeof(dex_size));  // Guarda el tamaño de la pokedex del binario para poder leerlo correctamente

    for (uint i = 0; i < dex_size; ++i) {   // Iteramos la cantidad de veces necesarias para leer todos los pares (este dato lo tenemos gracias a dex_size)
        ataques.clear();    // Vaciamos el vector de ataques que tenía contenidos los ataques anteriores
        read_string(nombre);    // Lee el nombre
        in.read(reinterpret_cast<char*>(&xp), sizeof(xp));  // Lee la experiencia
        

        read_string(tipo);  // Lee el tipo
        read_string(descripcion);   // Lee la descripción

        in.read(reinterpret_cast<char*>(&ataques_size), sizeof(ataques_size));  // Lee  el tamaño del vector de ataques
        
        for (uint j = 0; j < ataques_size; ++j) {   // Itera sobre cada ataque garantizando leer todos los escritos
            string nombre_ataque;
            uint dmg;
            read_string(nombre_ataque); // Lee el nombre del ataque
            in.read(reinterpret_cast<char*>(&dmg), sizeof(dmg));    // Lee el daño del ataque
            ataques.emplace_back(nombre_ataque, dmg);   // Inserta en el vector de ataques mediante emplace back
        }

        
        for (uint& xp_n : xp_niveles) { // Iteramos para cada nivel de experiencia (3)
            in.read(reinterpret_cast<char*>(&xp_n), sizeof(xp_n));  // Lee la experiencia y la guarda en el array gracias a la referencia
        }

        Pokedata.emplace(Pokemon(nombre, xp), PokemonInfo(tipo, descripcion, ataques, xp_niveles)); // Inserta en el mapa (pokedata) cada poke con su info usando emplace
    }
    in.close();     // Cierra el archiva
}