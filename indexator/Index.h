// Index.h
#ifndef INDEX_H
#define INDEX_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>
#include "Node.h"
#include "Element.h"

// Fonctions auxiliaires de conversion de types
// Ces fonctions sont en dehors de la classe pour éviter les problèmes de spécialisation
template <typename T>
T convertFromString(const std::string& str) {
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}

// Spécialisation pour std::string
template <>
inline std::string convertFromString<std::string>(const std::string& str) {
    return str;
}

// Spécialisation pour char
template <>
inline char convertFromString<char>(const std::string& str) {
    return str.empty() ? '\0' : str[0];
}

template <typename K, typename V>
class Index {
private:
    std::vector<Node<K, V>*> nodes;    // Collection de nœuds

public:
    // Constructeur
    Index() {}

    // Destructeur - libère la mémoire pour tous les nœuds
    ~Index() {
        for (auto node : nodes) {
            delete node;
        }
        nodes.clear();
    }

    // Retourne le nombre total d'éléments dans l'index
    int getNbElements() const;

    // Recherche un nœud par clé
    Node<K, V>* getNode(const K& key) const;

    // Ajoute un nouveau nœud avec la clé spécifiée
    void addNode(const K& key);

    // Supprime un nœud par clé
    bool deleteNode(const K& key);

    // Retourne tous les éléments correspondant à une clé
    std::vector<Element<K, V>*> getElements(const K& key) const;

    // Ajoute un élément à l'index
    void addElement(Element<K, V>* element);

    // Supprime un élément de l'index
    bool deleteElement(Element<K, V>* element);

    // Construit l'index à partir d'un fichier au format "key ; value"
    bool buildFromFile(const std::string& filename);

    // Affiche l'index (pour débogage)
    friend std::ostream& operator<<(std::ostream& os, const Index<K, V>& index) {
        os << "Index{" << std::endl;
        for (const auto& node : index.nodes) {
            os << "  " << *node << std::endl;
        }
        os << "}";
        return os;
    }
};

// Implémentation des méthodes de la classe Index

template <typename K, typename V>
int Index<K, V>::getNbElements() const {
    int count = 0;
    for (const auto& node : nodes) {
        count += node->getNbElements();
    }
    return count;
}

template <typename K, typename V>
Node<K, V>* Index<K, V>::getNode(const K& key) const {
    auto it = std::find_if(nodes.begin(), nodes.end(),
                         [&key](const Node<K, V>* node) {
                             return node->getKey() == key;
                         });

    if (it != nodes.end()) {
        return *it;
    }

    return nullptr;  // Nœud non trouvé
}

template <typename K, typename V>
void Index<K, V>::addNode(const K& key) {
    // Vérifier si un nœud avec cette clé existe déjà
    if (getNode(key) != nullptr) {
        return;  // Ne pas ajouter de doublons
    }

    // Créer et ajouter un nouveau nœud
    Node<K, V>* newNode = new Node<K, V>(key);
    nodes.push_back(newNode);

    // Trier les nœuds par clé
    std::sort(nodes.begin(), nodes.end(),
             [](const Node<K, V>* a, const Node<K, V>* b) {
                 return a->getKey() < b->getKey();
             });
}

template <typename K, typename V>
bool Index<K, V>::deleteNode(const K& key) {
    auto it = std::find_if(nodes.begin(), nodes.end(),
                         [&key](const Node<K, V>* node) {
                             return node->getKey() == key;
                         });

    if (it != nodes.end()) {
        Node<K, V>* node = *it;
        nodes.erase(it);
        delete node;
        return true;
    }

    return false;  // Nœud non trouvé
}

template <typename K, typename V>
std::vector<Element<K, V>*> Index<K, V>::getElements(const K& key) const {
    Node<K, V>* node = getNode(key);

    if (node != nullptr) {
        return node->getAllElements();
    }

    return std::vector<Element<K, V>*>();  // Retourne une collection vide si aucun nœud trouvé
}

template <typename K, typename V>
void Index<K, V>::addElement(Element<K, V>* element) {
    const K& elementKey = element->getKey();
    Node<K, V>* node = getNode(elementKey);

    // Si aucun nœud n'existe pour cette clé, en créer un
    if (node == nullptr) {
        addNode(elementKey);
        node = getNode(elementKey);
    }

    // Ajouter l'élément au nœud
    node->addElement(element);
}

template <typename K, typename V>
bool Index<K, V>::deleteElement(Element<K, V>* element) {
    const K& elementKey = element->getKey();
    Node<K, V>* node = getNode(elementKey);

    if (node == nullptr) {
        return false;  // Aucun nœud trouvé pour cette clé
    }

    bool deleted = node->deleteElement(element);

    // Si le nœud est vide après suppression, le supprimer aussi
    if (deleted && node->getNbElements() == 0) {
        deleteNode(elementKey);
    }

    return deleted;
}

// Méthode pour construire l'index à partir d'un fichier
template <typename K, typename V>
bool Index<K, V>::buildFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ignorer les lignes vides
        if (line.empty()) {
            continue;
        }

        // Trouver le séparateur ';'
        size_t separatorPos = line.find(';');
        if (separatorPos == std::string::npos) {
            std::cerr << "Avertissement: Ligne mal formatée ignorée: " << line << std::endl;
            continue;
        }

        // Extraire la clé et la valeur
        std::string keyStr = line.substr(0, separatorPos);
        std::string valueStr = line.substr(separatorPos + 1);

        // Supprimer les espaces en début et fin
        keyStr.erase(0, keyStr.find_first_not_of(" \t"));
        keyStr.erase(keyStr.find_last_not_of(" \t") + 1);
        valueStr.erase(0, valueStr.find_first_not_of(" \t"));
        valueStr.erase(valueStr.find_last_not_of(" \t") + 1);

        try {
            // Convertir la clé et la valeur aux types K et V
            K key = convertFromString<K>(keyStr);
            V value = convertFromString<V>(valueStr);

            // Créer et ajouter l'élément à l'index
            Element<K, V>* element = new Element<K, V>(key, value);
            addElement(element);
        }
        catch (const std::exception& e) {
            std::cerr << "Erreur lors de la conversion: " << e.what()
                      << " pour la ligne: " << line << std::endl;
        }
    }

    file.close();
    return true;
}

#endif // INDEX_H