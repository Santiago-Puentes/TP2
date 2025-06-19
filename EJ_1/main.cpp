#include "poke.hpp"

int main(){
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
    {
        Pokedex dex1("save");
        dex1[poke1] = info1;
        dex1[poke2] = info2;
        dex1[poke3] = info3;
        dex1.mostrarTodos();
    }

    Pokedex dex2("save");
    dex2.mostrarTodos();
}