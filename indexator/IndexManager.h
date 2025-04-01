#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H

#include <iostream>
#include <string>
#include <memory>
#include <variant>
#include <type_traits>
#include "Element.h"
#include "Node.h"
#include "Index.h"

// Classe pour gérer différents types d'index
class IndexManager {
private:
    enum class IndexType {
        NONE,
        CHAR_STRING,
        INT_STRING,
        INT_INT
    };

    Index<char, std::string>* charStringIndex = nullptr;
    Index<int, std::string>* intStringIndex = nullptr;
    Index<int, int>* intIntIndex = nullptr;
    IndexType currentType = IndexType::NONE;

    // Libère la mémoire des index
    void clearIndices() {
        if (charStringIndex) {
            delete charStringIndex;
            charStringIndex = nullptr;
        }
        if (intStringIndex) {
            delete intStringIndex;
            intStringIndex = nullptr;
        }
        if (intIntIndex) {
            delete intIntIndex;
            intIntIndex = nullptr;
        }
        currentType = IndexType::NONE;
    }

public:
    // Constructeur
    IndexManager() {}

    // Destructeur
    ~IndexManager() {
        clearIndices();
    }

    // Méthodes pour charger différents types d'index existants
    bool loadCharStringIndex(const std::string& filename) {
        clearIndices();
        charStringIndex = new Index<char, std::string>();
        if (charStringIndex->loadFromFile(filename)) {
            currentType = IndexType::CHAR_STRING;
            return true;
        }
        delete charStringIndex;
        charStringIndex = nullptr;
        return false;
    }

    bool loadIntStringIndex(const std::string& filename) {
        clearIndices();
        intStringIndex = new Index<int, std::string>();
        if (intStringIndex->loadFromFile(filename)) {
            currentType = IndexType::INT_STRING;
            return true;
        }
        delete intStringIndex;
        intStringIndex = nullptr;
        return false;
    }

    bool loadIntIntIndex(const std::string& filename) {
        clearIndices();
        intIntIndex = new Index<int, int>();
        if (intIntIndex->loadFromFile(filename)) {
            currentType = IndexType::INT_INT;
            return true;
        }
        delete intIntIndex;
        intIntIndex = nullptr;
        return false;
    }

    // Afficher l'index courant
    void displayCurrentIndex() const {
        if (currentType == IndexType::NONE) {
            std::cout << "Aucun index n'est actuellement chargé." << std::endl;
            return;
        }

        std::cout << "Index de type ";
        switch (currentType) {
            case IndexType::CHAR_STRING:
                std::cout << "Char/String:" << std::endl << *charStringIndex << std::endl;
                break;
            case IndexType::INT_STRING:
                std::cout << "Int/String:" << std::endl << *intStringIndex << std::endl;
                break;
            case IndexType::INT_INT:
                std::cout << "Int/Int:" << std::endl << *intIntIndex << std::endl;
                break;
            default:
                std::cout << "inconnu." << std::endl;
                break;
        }
    }

    // Rechercher des éléments par clé
    void searchByKey() {
        if (currentType == IndexType::NONE) {
            std::cout << "Aucun index n'est actuellement chargé." << std::endl;
            return;
        }

        switch (currentType) {
            case IndexType::CHAR_STRING: {
                char key;
                std::cout << "Entrez la clé à rechercher (un caractère): ";
                std::cin >> key;
                std::cin.ignore();

                auto elements = charStringIndex->getElements(key);
                displayElements(elements);
                break;
            }
            case IndexType::INT_STRING: {
                int key;
                std::cout << "Entrez la clé à rechercher (un entier): ";
                std::cin >> key;
                std::cin.ignore();

                auto elements = intStringIndex->getElements(key);
                displayElements(elements);
                break;
            }
            case IndexType::INT_INT: {
                int key;
                std::cout << "Entrez la clé à rechercher (un entier): ";
                std::cin >> key;
                std::cin.ignore();

                auto elements = intIntIndex->getElements(key);
                displayElements(elements);
                break;
            }
            default:
                std::cout << "Type d'index inconnu." << std::endl;
                break;
        }
    }

    // Ajouter un élément à l'index
    void addElementToIndex() {
        if (currentType == IndexType::NONE) {
            std::cout << "Aucun index n'est actuellement chargé." << std::endl;
            return;
        }

        switch (currentType) {
            case IndexType::CHAR_STRING: {
                char key;
                std::string value;
                
                std::cout << "Entrez la clé (un caractère): ";
                std::cin >> key;
                std::cin.ignore();
                
                std::cout << "Entrez la valeur (une chaîne): ";
                std::getline(std::cin, value);

                auto* element = new Element<char, std::string>(key, value);
                charStringIndex->addElement(element);
                std::cout << "Élément ajouté avec succès." << std::endl;
                break;
            }
            case IndexType::INT_STRING: {
                int key;
                std::string value;
                
                std::cout << "Entrez la clé (un entier): ";
                std::cin >> key;
                std::cin.ignore();
                
                std::cout << "Entrez la valeur (une chaîne): ";
                std::getline(std::cin, value);

                auto* element = new Element<int, std::string>(key, value);
                intStringIndex->addElement(element);
                std::cout << "Élément ajouté avec succès." << std::endl;
                break;
            }
            case IndexType::INT_INT: {
                int key, value;
                
                std::cout << "Entrez la clé (un entier): ";
                std::cin >> key;
                std::cin.ignore();
                
                std::cout << "Entrez la valeur (un entier): ";
                std::cin >> value;
                std::cin.ignore();

                auto* element = new Element<int, int>(key, value);
                intIntIndex->addElement(element);
                std::cout << "Élément ajouté avec succès." << std::endl;
                break;
            }
            default:
                std::cout << "Type d'index inconnu." << std::endl;
                break;
        }
    }

    // Supprimer un nœud
    void deleteNodeFromIndex() {
        if (currentType == IndexType::NONE) {
            std::cout << "Aucun index n'est actuellement chargé." << std::endl;
            return;
        }

        switch (currentType) {
            case IndexType::CHAR_STRING: {
                char key;
                std::cout << "Entrez la clé du nœud à supprimer (un caractère): ";
                std::cin >> key;
                std::cin.ignore();

                if (charStringIndex->deleteNode(key)) {
                    std::cout << "Nœud supprimé avec succès." << std::endl;
                } else {
                    std::cout << "Nœud non trouvé." << std::endl;
                }
                break;
            }
            case IndexType::INT_STRING: {
                int key;
                std::cout << "Entrez la clé du nœud à supprimer (un entier): ";
                std::cin >> key;
                std::cin.ignore();

                if (intStringIndex->deleteNode(key)) {
                    std::cout << "Nœud supprimé avec succès." << std::endl;
                } else {
                    std::cout << "Nœud non trouvé." << std::endl;
                }
                break;
            }
            case IndexType::INT_INT: {
                int key;
                std::cout << "Entrez la clé du nœud à supprimer (un entier): ";
                std::cin >> key;
                std::cin.ignore();

                if (intIntIndex->deleteNode(key)) {
                    std::cout << "Nœud supprimé avec succès." << std::endl;
                } else {
                    std::cout << "Nœud non trouvé." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Type d'index inconnu." << std::endl;
                break;
        }
    }

    // Supprimer un élément
    void deleteElementFromIndex() {
        if (currentType == IndexType::NONE) {
            std::cout << "Aucun index n'est actuellement chargé." << std::endl;
            return;
        }

        switch (currentType) {
            case IndexType::CHAR_STRING: {
                char key;
                std::cout << "Entrez la clé de l'élément à supprimer (un caractère): ";
                std::cin >> key;
                std::cin.ignore();

                auto elements = charStringIndex->getElements(key);
                if (elements.empty()) {
                    std::cout << "Aucun élément trouvé avec cette clé." << std::endl;
                    return;
                }

                displayElements(elements);

                int index;
                std::cout << "Entrez le numéro de l'élément à supprimer: ";
                std::cin >> index;
                std::cin.ignore();

                if (index < 1 || index > static_cast<int>(elements.size())) {
                    std::cout << "Numéro d'élément invalide." << std::endl;
                    return;
                }

                if (charStringIndex->deleteElement(elements[index-1])) {
                    std::cout << "Élément supprimé avec succès." << std::endl;
                } else {
                    std::cout << "Échec de la suppression de l'élément." << std::endl;
                }
                break;
            }
            case IndexType::INT_STRING: {
                int key;
                std::cout << "Entrez la clé de l'élément à supprimer (un entier): ";
                std::cin >> key;
                std::cin.ignore();

                auto elements = intStringIndex->getElements(key);
                if (elements.empty()) {
                    std::cout << "Aucun élément trouvé avec cette clé." << std::endl;
                    return;
                }

                displayElements(elements);

                int index;
                std::cout << "Entrez le numéro de l'élément à supprimer: ";
                std::cin >> index;
                std::cin.ignore();

                if (index < 1 || index > static_cast<int>(elements.size())) {
                    std::cout << "Numéro d'élément invalide." << std::endl;
                    return;
                }

                if (intStringIndex->deleteElement(elements[index-1])) {
                    std::cout << "Élément supprimé avec succès." << std::endl;
                } else {
                    std::cout << "Échec de la suppression de l'élément." << std::endl;
                }
                break;
            }
            case IndexType::INT_INT: {
                int key;
                std::cout << "Entrez la clé de l'élément à supprimer (un entier): ";
                std::cin >> key;
                std::cin.ignore();

                auto elements = intIntIndex->getElements(key);
                if (elements.empty()) {
                    std::cout << "Aucun élément trouvé avec cette clé." << std::endl;
                    return;
                }

                displayElements(elements);

                int index;
                std::cout << "Entrez le numéro de l'élément à supprimer: ";
                std::cin >> index;
                std::cin.ignore();

                if (index < 1 || index > static_cast<int>(elements.size())) {
                    std::cout << "Numéro d'élément invalide." << std::endl;
                    return;
                }

                if (intIntIndex->deleteElement(elements[index-1])) {
                    std::cout << "Élément supprimé avec succès." << std::endl;
                } else {
                    std::cout << "Échec de la suppression de l'élément." << std::endl;
                }
                break;
            }
            default:
                std::cout << "Type d'index inconnu." << std::endl;
                break;
        }
    }

    // Afficher le nombre d'éléments dans l'index
    void countElements() const {
        if (currentType == IndexType::NONE) {
            std::cout << "Aucun index n'est actuellement chargé." << std::endl;
            return;
        }

        int count = 0;
        std::string type;
        
        switch (currentType) {
            case IndexType::CHAR_STRING:
                count = charStringIndex->getNbElements();
                type = "Char/String";
                break;
            case IndexType::INT_STRING:
                count = intStringIndex->getNbElements();
                type = "Int/String";
                break;
            case IndexType::INT_INT:
                count = intIntIndex->getNbElements();
                type = "Int/Int";
                break;
            default:
                type = "inconnu";
                break;
        }

        std::cout << "L'index de type " << type << " contient " << count << " élément(s)." << std::endl;
    }

    // Vérifier si un index est chargé
    bool isIndexLoaded() const {
        return currentType != IndexType::NONE;
    }

    // Méthode utilitaire pour afficher les éléments
    template <typename T>
    void displayElements(const std::vector<T>& elements) const {
        if (elements.empty()) {
            std::cout << "Aucun élément trouvé." << std::endl;
            return;
        }

        std::cout << "Éléments trouvés (" << elements.size() << ") :" << std::endl;
        for (size_t i = 0; i < elements.size(); ++i) {
            std::cout << (i + 1) << ". " << *elements[i] << std::endl;
        }
    }
};

#endif // INDEX_MANAGER_H