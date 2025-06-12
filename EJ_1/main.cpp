#include "poke.hpp"

int main(){
    // while(true) {
    //     cout << "Pokedex" << endl;
    //     cout << "Elija una opción a continuación (1 - 4)" << endl;
    //     cout << "1) Insertar Pokemon" << endl;
    //     cout << "2) Mostrar Pokemones registrados" << endl;
    //     cout << "3) Mostrar Pokemon específico" << endl;
    //     cout << "4) Insertar pokemones desde archivo" << endl;
    //     cout << "5) Salir" << endl;
    //     int eleccion;
    //     cin >> eleccion;

    //     switch (eleccion)
    //     {
    //     case 1:
    //         cout << "Insertar nombre del pokemon:" << endl;
    //         string input;
    //         cin >> input;
    //         cout << "Ingresar xp del pokemon" << endl;
    //         int xp;
    //         cin >> xp;
    //         Pokemon poke(input,xp);
    //         cout << "Ingresar tipo de pokemon" << endl;
    //         string type;
    //         cin >> type;
    //         cout << "Ingrese una descripción para el pokemon" << endl;
    //         string descripcion;
    //         cin >> descripcion;
    //         do {
    //             cout << "Ingrese ataques "
    //         }
    //         break;
    //     case 2:
    //         /* code */
    //         break;
    //     case 3:
    //         /* code */
    //         break;
    //     case 4:
    //         /* code */
    //         break;
    //     default:
    //         break;
    //     }    
    //}
    Pokemon poke1("Squirtle", 100);
    PokemonInfo info1("Agua", "Una tortuga pequeña que lanza chorros de agua.",  
                         {{"Pistola Agua", 4}, {"Hidrobomba", 6}, {"Danza Lluvia", 5}}, 
                         {0, 400, 1000}); 
    Pokemon poke2("Bulbasaur", 270);
    PokemonInfo info2("Planta", "Tiene una semilla en su lomo que crece con el tiempo", 
                         {{"Latigazo", 4}, {"Hoja Afilada", 6}, {"Rayo Solar", 5}}, 
                         {0, 300, 1100});  
    Pokemon poke3("Charmander", 633);
    PokemonInfo info3("Fuego", "Una lagartija con una llama en su cola", 
                         {{"Ascuas", 4}, {"Lanzallamas", 6}, {"Giro Fuego", 5}}, 
                         {0, 250, 1300});

    Pokedex dex("poke_save");
    dex[poke1] = info1;
    dex[poke2] = info2;
    dex[poke3] = info3;
}