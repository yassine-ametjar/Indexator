#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include "Element.h"
#include "Node.h"
#include "Index.h"

// Fonction pour créer un fichier de test
template <typename K, typename V>
void createTestFile(const std::string& filename,
                   const std::vector<std::pair<K, V>>& data,
                   const std::function<std::string(const K&)>& keyToString,
                   const std::function<std::string(const V&)>& valueToString) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible de créer le fichier " << filename << std::endl;
        return;
    }

    for (const auto& pair : data) {
        file << keyToString(pair.first) << " ; " << valueToString(pair.second) << std::endl;
    }

    file.close();
    std::cout << "Fichier " << filename << " créé avec succès." << std::endl;
}

// Démonstration de la classe Element
void demoElement() {
    std::cout << "\n========== DÉMONSTRATION DE LA CLASSE ELEMENT ==========\n" << std::endl;

    // Création d'éléments avec différents types
    Element<char, std::string> e1('a', "Ahmed");
    Element<int, std::string> e2(12, "Simon");
    Element<int, int> e3(200, 267);

    // Affichage des éléments
    std::cout << "Éléments créés:" << std::endl;
    std::cout << "e1: " << e1 << std::endl;
    std::cout << "e2: " << e2 << std::endl;
    std::cout << "e3: " << e3 << std::endl;

    // Accès aux propriétés
    std::cout << "\nAccès aux propriétés:" << std::endl;
    std::cout << "e1.getKey(): " << e1.getKey() << ", e1.getValue(): " << e1.getValue() << std::endl;
    std::cout << "e2.getKey(): " << e2.getKey() << ", e2.getValue(): " << e2.getValue() << std::endl;
    std::cout << "e3.getKey(): " << e3.getKey() << ", e3.getValue(): " << e3.getValue() << std::endl;

    // Modification des valeurs
    std::cout << "\nModification des valeurs:" << std::endl;
    e1.setValue("Adam");
    std::cout << "Après e1.setValue(\"Adam\"): " << e1 << std::endl;

    // Comparaisons
    Element<char, std::string> e4('a', "Anais");
    Element<char, std::string> e5('a', "Adam");  // Même valeur que e1 modifiée

    std::cout << "\nComparaisons:" << std::endl;
    std::cout << "e1 == e5: " << (e1 == e5 ? "vrai" : "faux") << std::endl;
    std::cout << "e1 < e4: " << (e1 < e4 ? "vrai" : "faux") << std::endl;
    std::cout << "e4 < e1: " << (e4 < e1 ? "vrai" : "faux") << std::endl;
}

// Démonstration de la classe Node
void demoNode() {
    std::cout << "\n========== DÉMONSTRATION DE LA CLASSE NODE ==========\n" << std::endl;

    // 1. Création d'un nœud
    Node<char, std::string>* node = new Node<char, std::string>('a');
    std::cout << "Nœud créé avec la clé 'a'" << std::endl;
    std::cout << "Clé du nœud: " << node->getKey() << std::endl;
    std::cout << "Nombre d'éléments initial: " << node->getNbElements() << std::endl;

    // 2. Ajout d'éléments au nœud
    std::cout << "\nAjout d'éléments au nœud..." << std::endl;

    Element<char, std::string>* e1 = new Element<char, std::string>('a', "Anatole");
    Element<char, std::string>* e2 = new Element<char, std::string>('a', "Ahmed");
    Element<char, std::string>* e3 = new Element<char, std::string>('a', "Anais");

    node->addElement(e1);
    std::cout << "Après ajout de \"Anatole\": " << node->getNbElements() << " élément(s)" << std::endl;

    node->addElement(e2);
    std::cout << "Après ajout de \"Ahmed\": " << node->getNbElements() << " élément(s)" << std::endl;

    node->addElement(e3);
    std::cout << "Après ajout de \"Anais\": " << node->getNbElements() << " élément(s)" << std::endl;

    // 3. Vérification du tri automatique des éléments
    std::cout << "\nVérification du tri des éléments par valeur:" << std::endl;
    auto elements = node->getAllElements();
    for (size_t i = 0; i < elements.size(); ++i) {
        std::cout << "  " << (i+1) << ". " << elements[i]->getValue() << std::endl;
    }

    // 4. Test de la gestion des erreurs - Ajout d'un élément avec une clé incorrecte
    std::cout << "\nTentative d'ajout d'un élément avec une clé incorrecte:" << std::endl;
    Element<char, std::string>* badElement = new Element<char, std::string>('b', "Beatrice");

    try {
        node->addElement(badElement);
        std::cout << "ERREUR: L'exception attendue n'a pas été levée!" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "Exception correctement levée: " << e.what() << std::endl;
        delete badElement;  // Nettoyage car l'élément n'a pas été ajouté
    }

    // 5. Recherche d'éléments par clé
    std::cout << "\nRecherche d'éléments avec la clé 'a':" << std::endl;
    auto matchingElements = node->getElements('a');
    for (const auto& element : matchingElements) {
        std::cout << "  " << element->getValue() << std::endl;
    }

    std::cout << "\nRecherche d'éléments avec la clé 'b' (devrait être vide):" << std::endl;
    auto nonMatchingElements = node->getElements('b');
    if (nonMatchingElements.empty()) {
        std::cout << "  Aucun élément trouvé, comme attendu." << std::endl;
    }

    // 6. Suppression d'un élément
    std::cout << "\nSuppression de l'élément \"Ahmed\"..." << std::endl;
    bool deleted = node->deleteElement(e2);
    std::cout << (deleted ? "Élément supprimé avec succès" : "Échec de la suppression") << std::endl;
    std::cout << "Nombre d'éléments après suppression: " << node->getNbElements() << std::endl;

    std::cout << "\nÉléments du nœud après suppression:" << std::endl;
    elements = node->getAllElements();
    for (const auto& element : elements) {
        std::cout << "  " << element->getValue() << std::endl;
    }

    // 7. Libération de la mémoire
    delete node; // Le destructeur de Node libère automatiquement la mémoire des éléments restants
    std::cout << "\nNœud supprimé et mémoire libérée." << std::endl;
}

// Démonstration de l'index char/string (Figure 1)
void demoCharStringIndex() {
    std::cout << "\n========== DÉMONSTRATION INDEX CHAR/STRING (FIGURE 1) ==========\n" << std::endl;

    // 1. Création du fichier de test
    std::vector<std::pair<char, std::string>> data = {
        {'a', "Ahmed"},
        {'a', "Anatole"},
        {'a', "Anais"},
        {'b', "Beatrice"},
        {'y', "Yassine"},
        {'z', "Zakary"},
        {'z', "Zoe"}
    };

    createTestFile<char, std::string>("prenoms.txt", data,
                              [](const char& c) { return std::string(1, c); },
                              [](const std::string& s) { return s; });

    // 2. Création de l'index à partir du fichier
    std::cout << "\nCréation de l'index à partir du fichier prenoms.txt..." << std::endl;
    Index<char, std::string> index;
    if (index.buildFromFile("prenoms.txt")) {
        std::cout << "Index créé avec succès!" << std::endl;
        std::cout << "Nombre total d'éléments: " << index.getNbElements() << std::endl;
        std::cout << index << std::endl;
    }

    // 3. Accès direct à un nœud de l'index
    std::cout << "\nAccès direct au nœud 'a':" << std::endl;
    Node<char, std::string>* nodeA = index.getNode('a');
    if (nodeA) {
        std::cout << "Nœud trouvé: " << *nodeA << std::endl;
        std::cout << "Clé du nœud: " << nodeA->getKey() << std::endl;
        std::cout << "Nombre d'éléments: " << nodeA->getNbElements() << std::endl;

        std::cout << "Éléments du nœud 'a':" << std::endl;
        auto elements = nodeA->getAllElements();
        for (const auto& element : elements) {
            std::cout << "  " << element->getValue() << std::endl;
        }
    }

    // 4. Recherche d'éléments par clé
    std::cout << "\nRecherche d'éléments avec différentes clés:" << std::endl;
    for (char c : {'a', 'z', 'x'}) {  // 'x' n'existe pas
        std::cout << "Clé '" << c << "':" << std::endl;
        auto elements = index.getElements(c);
        if (elements.empty()) {
            std::cout << "  Aucun élément trouvé." << std::endl;
        } else {
            for (const auto& element : elements) {
                std::cout << "  " << element->getValue() << std::endl;
            }
        }
    }

    // 5. Ajout d'un nouvel élément avec une clé existante
    std::cout << "\nAjout d'un élément avec une clé existante ('a'):" << std::endl;
    Element<char, std::string>* newElement1 = new Element<char, std::string>('a', "Alice");
    index.addElement(newElement1);
    std::cout << "Après ajout:" << std::endl;

    nodeA = index.getNode('a');
    if (nodeA) {
        std::cout << "Nœud 'a': " << *nodeA << std::endl;
        auto elements = nodeA->getAllElements();
        std::cout << "Éléments du nœud 'a' (triés par valeur):" << std::endl;
        for (const auto& element : elements) {
            std::cout << "  " << element->getValue() << std::endl;
        }
    }

    // 6. Ajout d'un élément avec une nouvelle clé
    std::cout << "\nAjout d'un élément avec une nouvelle clé ('c'):" << std::endl;
    Element<char, std::string>* newElement2 = new Element<char, std::string>('c', "Claire");
    index.addElement(newElement2);
    std::cout << "Après ajout:" << std::endl;
    std::cout << index << std::endl;

    // 7. Suppression d'un élément
    std::cout << "\nSuppression de l'élément \"Beatrice\" (clé 'b'):" << std::endl;
    auto bElements = index.getElements('b');
    if (!bElements.empty()) {
        Element<char, std::string>* elementToDelete = bElements[0];
        bool deleted = index.deleteElement(elementToDelete);
        std::cout << (deleted ? "Élément supprimé avec succès" : "Échec de la suppression") << std::endl;
        std::cout << "Index après suppression (le nœud 'b' devrait avoir disparu):" << std::endl;
        std::cout << index << std::endl;
    }

    // 8. Suppression d'un élément mais pas du nœud
    std::cout << "\nSuppression de l'élément \"Alice\" (clé 'a'):" << std::endl;
    auto aElements = index.getElements('a');
    Element<char, std::string>* elementToFind = nullptr;

    for (auto& element : aElements) {
        if (element->getValue() == "Alice") {
            elementToFind = element;
            break;
        }
    }

    if (elementToFind) {
        bool deleted = index.deleteElement(elementToFind);
        std::cout << (deleted ? "Élément supprimé avec succès" : "Échec de la suppression") << std::endl;
        std::cout << "Index après suppression (le nœud 'a' devrait encore exister):" << std::endl;
        std::cout << index << std::endl;
    }
}

// Démonstration de l'index int/string (Figure 2)
void demoIntStringIndex() {
    std::cout << "\n========== DÉMONSTRATION INDEX INT/STRING (FIGURE 2) ==========\n" << std::endl;

    // 1. Création du fichier de test
    std::vector<std::pair<int, std::string>> data = {
        {6, "Ahmed"},
        {6, "Antoine"},
        {6, "Chloe"},
        {10, "Michel"},
        {12, "Eloise"},
        {12, "Simon"}
    };

    createTestFile<int, std::string>("notes.txt", data,
                              [](const int& i) { return std::to_string(i); },
                              [](const std::string& s) { return s; });

    // 2. Création de l'index
    std::cout << "\nCréation de l'index à partir du fichier notes.txt..." << std::endl;
    Index<int, std::string> noteIndex;
    if (noteIndex.buildFromFile("notes.txt")) {
        std::cout << "Index créé avec succès!" << std::endl;
        std::cout << "Nombre total d'éléments: " << noteIndex.getNbElements() << std::endl;
        std::cout << noteIndex << std::endl;
    }

    // 3. Tests de recherche par note
    std::cout << "\nRecherche d'élèves par note:" << std::endl;
    for (int note : {6, 10, 12, 8}) {  // 8 n'existe pas
        std::cout << "Note " << note << ":" << std::endl;
        auto elements = noteIndex.getElements(note);
        if (elements.empty()) {
            std::cout << "  Aucun élève trouvé." << std::endl;
        } else {
            for (const auto& element : elements) {
                std::cout << "  " << element->getValue() << std::endl;
            }
        }
    }

    // 4. Manipulation directe d'un nœud
    std::cout << "\nManipulation directe du nœud correspondant à la note 12:" << std::endl;
    Node<int, std::string>* node12 = noteIndex.getNode(12);
    if (node12) {
        std::cout << "Nœud trouvé: " << *node12 << std::endl;

        std::cout << "Ajout direct d'un élément au nœud 12:" << std::endl;
        Element<int, std::string>* newStudent = new Element<int, std::string>(12, "Thomas");
        node12->addElement(newStudent);

        std::cout << "Nœud après ajout: " << *node12 << std::endl;
        std::cout << "Index après modification directe d'un nœud:" << std::endl;
        std::cout << noteIndex << std::endl;
    }

    // 5. Suppression complète d'un nœud
    std::cout << "\nSuppression directe du nœud avec la note 10:" << std::endl;
    bool deleted = noteIndex.deleteNode(10);
    std::cout << (deleted ? "Nœud supprimé avec succès" : "Échec de la suppression") << std::endl;
    std::cout << "Index après suppression:" << std::endl;
    std::cout << noteIndex << std::endl;
}

// Démonstration de l'index int/int (Figure 3)
void demoIntIntIndex() {
    std::cout << "\n========== DÉMONSTRATION INDEX INT/INT (FIGURE 3) ==========\n" << std::endl;

    // 1. Création du fichier de test
    std::vector<std::pair<int, int>> data = {
        {1, 2},
        {1, 7},
        {1, 9},
        {10, 13},
        {200, 223},
        {200, 267}
    };

    createTestFile<int, int>("nombres.txt", data,
                          [](const int& i) { return std::to_string(i); },
                          [](const int& i) { return std::to_string(i); });

    // 2. Création de l'index
    std::cout << "\nCréation de l'index à partir du fichier nombres.txt..." << std::endl;
    Index<int, int> nombreIndex;
    if (nombreIndex.buildFromFile("nombres.txt")) {
        std::cout << "Index créé avec succès!" << std::endl;
        std::cout << "Nombre total d'éléments: " << nombreIndex.getNbElements() << std::endl;
        std::cout << nombreIndex << std::endl;
    }

    // 3. Tests des différentes méthodes de l'index
    std::cout << "\nRecherche de nombres par borne:" << std::endl;
    for (int borne : {1, 10, 200}) {
        std::cout << "Borne " << borne << ":" << std::endl;
        auto elements = nombreIndex.getElements(borne);

        std::cout << "  Nombres supérieurs à " << borne << ": ";
        for (const auto& element : elements) {
            std::cout << element->getValue() << " ";
        }
        std::cout << std::endl;
    }

    // 4. Ajout et suppression combinés
    std::cout << "\nTest d'ajout et de suppression combinés:" << std::endl;

    // Ajout d'éléments avec une nouvelle clé
    std::cout << "Ajout de deux éléments avec la borne 50:" << std::endl;
    Element<int, int>* e1 = new Element<int, int>(50, 55);
    Element<int, int>* e2 = new Element<int, int>(50, 67);

    nombreIndex.addElement(e1);
    nombreIndex.addElement(e2);
    std::cout << "Index après ajout:" << std::endl;
    std::cout << nombreIndex << std::endl;

    // Suppression de l'un des deux éléments
    std::cout << "\nSuppression de l'élément 55 (borne 50):" << std::endl;
    bool deleted = nombreIndex.deleteElement(e1);
    std::cout << (deleted ? "Élément supprimé avec succès" : "Échec de la suppression") << std::endl;
    std::cout << "Index après suppression:" << std::endl;
    std::cout << nombreIndex << std::endl;

    // Suppression du dernier élément de la borne 50
    std::cout << "\nSuppression de l'élément 67 (borne 50, dernier élément):" << std::endl;
    deleted = nombreIndex.deleteElement(e2);
    std::cout << (deleted ? "Élément supprimé avec succès" : "Échec de la suppression") << std::endl;
    std::cout << "Index après suppression (le nœud 50 devrait avoir disparu):" << std::endl;
    std::cout << nombreIndex << std::endl;
}

int main() {
    std::cout << "===============================================" << std::endl;
    std::cout << "     PROGRAMME DE TEST COMPLET DE L'INDEX     " << std::endl;
    std::cout << "===============================================" << std::endl;

    // Tests des classes individuelles
    demoElement();
    demoNode();

    // Tests des différents types d'index
    demoCharStringIndex();
    demoIntStringIndex();
    demoIntIntIndex();

    std::cout << "\n===============================================" << std::endl;
    std::cout << "     TOUS LES TESTS TERMINÉS AVEC SUCCÈS      " << std::endl;
    std::cout << "===============================================" << std::endl;

    return 0;
}