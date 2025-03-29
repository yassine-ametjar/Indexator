// test_node.cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include "Element.h"
#include "Node.h"

// Fonction utilitaire pour vérifier les assertions
#define TEST_ASSERT(condition, message) \
    if (!(condition)) { \
        std::cerr << "ÉCHEC: " << message << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        exit(1); \
    } else { \
        std::cout << "SUCCÈS: " << message << std::endl; \
    }

// Test principal pour la classe Node
template <typename K, typename V>
void testNode(const std::string& testName, const K& nodeKey,
              const std::vector<std::pair<K, V>>& elementData) {
    std::cout << "\n=== Test Node avec " << testName << " ===\n";

    // Création du nœud
    Node<K, V>* node = new Node<K, V>(nodeKey);
    std::cout << "Nœud créé: " << *node << std::endl;

    TEST_ASSERT(node->getKey() == nodeKey, "La clé du nœud est correcte");
    TEST_ASSERT(node->getNbElements() == 0, "Le nœud est initialement vide");

    // Test d'ajout d'éléments
    std::cout << "\nAjout d'éléments..." << std::endl;
    for (const auto& pair : elementData) {
        try {
            Element<K, V>* element = new Element<K, V>(pair.first, pair.second);
            std::cout << "Tentative d'ajout: " << *element << std::endl;

            if (pair.first == nodeKey) {
                node->addElement(element);
                std::cout << "Élément ajouté avec succès" << std::endl;
            } else {
                // On s'attend à ce qu'une exception soit lancée
                bool exceptionCaught = false;
                try {
                    node->addElement(element);
                } catch (const std::invalid_argument& e) {
                    exceptionCaught = true;
                    delete element; // Nettoyage car l'élément n'a pas été ajouté au nœud
                    std::cout << "Exception attendue attrapée: " << e.what() << std::endl;
                }

                TEST_ASSERT(exceptionCaught, "Exception levée pour clé incorrecte");
            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur inattendue: " << e.what() << std::endl;
        }
    }

    // Vérification du nombre d'éléments
    int expectedCount = std::count_if(elementData.begin(), elementData.end(),
                                      [nodeKey](const std::pair<K, V>& p) { return p.first == nodeKey; });

    TEST_ASSERT(node->getNbElements() == expectedCount,
                "Le nombre d'éléments est correct après les ajouts");

    // Vérification que les éléments sont triés par valeur
    auto elements = node->getAllElements();
    std::cout << "\nVérification du tri des éléments:" << std::endl;
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << "  " << *elements[i] << std::endl;
        if (i > 0) {
            TEST_ASSERT(elements[i-1]->getValue() <= elements[i]->getValue(),
                        "Les éléments sont bien triés par valeur");
        }
    }

    // Test de getElements avec la clé du nœud
    auto matchingElements = node->getElements(nodeKey);
    TEST_ASSERT(matchingElements.size() == expectedCount,
                "getElements retourne tous les éléments pour la clé du nœud");

    // Test de getElements avec une clé différente
    K differentKey;
    // Trouver une clé différente de nodeKey
    for (const auto& pair : elementData) {
        if (pair.first != nodeKey) {
            differentKey = pair.first;
            break;
        }
    }

    auto nonMatchingElements = node->getElements(differentKey);
    TEST_ASSERT(nonMatchingElements.empty(),
                "getElements retourne une collection vide pour une clé différente");

    // Test de suppression d'élément
    if (!elements.empty()) {
        std::cout << "\nTest de suppression d'élément:" << std::endl;
        Element<K, V>* elementToDelete = elements[0];
        std::cout << "Suppression de: " << *elementToDelete << std::endl;

        bool deleteResult = node->deleteElement(elementToDelete);
        TEST_ASSERT(deleteResult, "deleteElement retourne true pour un élément existant");
        TEST_ASSERT(node->getNbElements() == expectedCount - 1,
                    "Le nombre d'éléments est décrémenté après suppression");

        // Test de suppression d'un élément inexistant
        Element<K, V> nonExistentElement(nodeKey, V()); // Créer un élément avec une valeur par défaut
        std::cout << "Tentative de suppression d'un élément inexistant" << std::endl;

        deleteResult = node->deleteElement(&nonExistentElement);
        TEST_ASSERT(!deleteResult, "deleteElement retourne false pour un élément inexistant");
    }

    // Nettoyage
    delete node;
    std::cout << "\nNœud supprimé (et tous ses éléments avec lui)" << std::endl;
}

int main() {
    std::cout << "=== Programme de test pour la classe Node ===\n";

    // Test 1: Nœud avec clé char et valeurs string
    std::vector<std::pair<char, std::string>> charStringData = {
        {'a', "Ahmed"},
        {'a', "Anais"},
        {'a', "Anatole"},
        {'b', "Beatrice"},  // Clé différente, devrait être rejetée
        {'a', "Alice"}
    };
    testNode<char, std::string>("char/string", 'a', charStringData);

    // Test 2: Nœud avec clé int et valeurs string
    std::vector<std::pair<int, std::string>> intStringData = {
        {12, "Simon"},
        {12, "Eloise"},
        {6, "Ahmed"},       // Clé différente, devrait être rejetée
        {12, "Arnaud"}
    };
    testNode<int, std::string>("int/string", 12, intStringData);

    // Test 3: Nœud avec clé int et valeurs int
    std::vector<std::pair<int, int>> intIntData = {
        {200, 267},
        {200, 223},
        {1, 2},             // Clé différente, devrait être rejetée
        {200, 245}
    };
    testNode<int, int>("int/int", 200, intIntData);

    std::cout << "\nTous les tests sont terminés avec succès !\n";
    return 0;
}