#include <iostream>
#include <string>
#include <limits>
#include "Element.h"
#include "Node.h"
#include "Index.h"
#include "IndexManager.h"

// Fonction pour effacer le buffer d'entrée
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Fonction principale avec menu interactif
int main() {
    IndexManager manager;
    int choice = 0;
    bool running = true;
    std::string filename;

    std::cout << "=== INDEXATOR - Gestionnaire d'index ===" << std::endl;

    while (running) {
        std::cout << "\nMenu Principal:\n";
        std::cout << "1. Charger un index Char/String existant\n";
        std::cout << "2. Charger un index Int/String existant\n";
        std::cout << "3. Charger un index Int/Int existant\n";
        std::cout << "4. Afficher l'index courant\n";
        std::cout << "5. Rechercher par clé\n";
        std::cout << "6. Ajouter un élément\n";
        std::cout << "7. Supprimer un élément\n";
        std::cout << "8. Supprimer un nœud\n";
        std::cout << "9. Compter les éléments\n";
        std::cout << "0. Quitter\n";
        std::cout << "Votre choix: ";
        std::cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1: {  // Charger un index Char/String existant
                std::cout << "Entrez le nom du fichier à charger: ";
                std::getline(std::cin, filename);

                if (manager.loadCharStringIndex(filename)) {
                    std::cout << "Index Char/String chargé avec succès." << std::endl;
                } else {
                    std::cout << "Erreur lors du chargement de l'index." << std::endl;
                }
                break;
            }

            case 2: {  // Charger un index Int/String existant
                std::cout << "Entrez le nom du fichier à charger: ";
                std::getline(std::cin, filename);

                if (manager.loadIntStringIndex(filename)) {
                    std::cout << "Index Int/String chargé avec succès." << std::endl;
                } else {
                    std::cout << "Erreur lors du chargement de l'index." << std::endl;
                }
                break;
            }

            case 3: {  // Charger un index Int/Int existant
                std::cout << "Entrez le nom du fichier à charger: ";
                std::getline(std::cin, filename);

                if (manager.loadIntIntIndex(filename)) {
                    std::cout << "Index Int/Int chargé avec succès." << std::endl;
                } else {
                    std::cout << "Erreur lors du chargement de l'index." << std::endl;
                }
                break;
            }

            case 4:  // Afficher l'index courant
                manager.displayCurrentIndex();
                break;

            case 5:  // Rechercher par clé
                if (manager.isIndexLoaded()) {
                    manager.searchByKey();
                } else {
                    std::cout << "Aucun index n'est chargé." << std::endl;
                }
                break;

            case 6:  // Ajouter un élément
                if (manager.isIndexLoaded()) {
                    manager.addElementToIndex();
                } else {
                    std::cout << "Aucun index n'est chargé." << std::endl;
                }
                break;

            case 7:  // Supprimer un élément
                if (manager.isIndexLoaded()) {
                    manager.deleteElementFromIndex();
                } else {
                    std::cout << "Aucun index n'est chargé." << std::endl;
                }
                break;

            case 8:  // Supprimer un nœud
                if (manager.isIndexLoaded()) {
                    manager.deleteNodeFromIndex();
                } else {
                    std::cout << "Aucun index n'est chargé." << std::endl;
                }
                break;

            case 9:  // Compter les éléments
                if (manager.isIndexLoaded()) {
                    manager.countElements();
                } else {
                    std::cout << "Aucun index n'est chargé." << std::endl;
                }
                break;

            case 0:  // Quitter
                std::cout << "Au revoir!" << std::endl;
                running = false;
                break;

            default:
                std::cout << "Option invalide. Veuillez réessayer." << std::endl;
                break;
        }
    }

    return 0;
}