#include "poke.hpp"

int main(){
    // Inicializamos los pokemones ejemplo de la cosnigna
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

    cout << "Creando pokedex...\n";
    // Ahora si inicializamos la pokedex, con guardado binario
    {
        Pokedex dex1("save");
        dex1[poke1] = info1;    // Se hacen los respectivos inserts
        dex1[poke2] = info2;
        dex1[poke3] = info3;
        dex1.mostrarTodos();    // Mostramos la pokedex
    }   //  Una vez se sale del scope se guarda
    //Ahora restoreamos la pokedex anterior y mostramos su contenido
    cout << "Recuperando pokedex y buscando pokemones...\n";
    {
        Pokedex dex2("save");
        dex2.mostrar(Pokemon("Squirtle", 870)); // Mostramos un pokemon existente
        dex2[poke2] = info2; // Esto no modifica nada al ser la misma info vinculada el binario tambien queda intacto
        //dex2[poke2] = info1; // Cambiaría los datos del poke2 por los del poke 1, lo cual es peligroso
        dex2.mostrar(Pokemon("Pikachu", 390));  // Mostramos un pokemon inexistente
        dex2.mostrarTodos(); // Mostramos todo lo recuperado
    }   // Se guarda de nuevo
}