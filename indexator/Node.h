#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Element.h"

template <typename K, typename V>
class Node {
private:
    K key;                              // Clé de recherche du nœud
    std::vector<Element<K, V>*> elements; // Collection d'éléments du nœud

public:
    // Constructeur
    Node(const K& k) : key(k) {}

    // Destructeur - libère la mémoire allouée pour les éléments
    ~Node() {
        for (auto element : elements) {
            delete element;
        }
        elements.clear();
    }

    // Accesseurs
    const K& getKey() const { return key; }

    int getNbElements() const { return elements.size(); }

    // Retourne tous les éléments du nœud
    std::vector<Element<K, V>*> getAllElements() const {
        return elements;
    }

    // Retourne les éléments dont la clé correspond à k
    std::vector<Element<K, V>*> getElements(const K& k) const {
        // Dans un nœud, tous les éléments ont la même clé (celle du nœud)
        // Donc si k est différent de la clé du nœud, aucun élément ne correspond
        if (k != key) {
            return std::vector<Element<K, V>*>();
        }
        return elements;
    }

    // Ajoute un élément au nœud
    void addElement(Element<K, V>* element) {
        // Vérifier que la clé de l'élément correspond à celle du nœud
        if (element->getKey() != key) {
            throw std::invalid_argument("La clé de l'élément ne correspond pas à celle du nœud");
        }

        // Ajouter l'élément
        elements.push_back(element);

        // Trier les éléments par valeur
        std::sort(elements.begin(), elements.end(),
            [](const Element<K, V>* a, const Element<K, V>* b) {
                return a->getValue() < b->getValue();
            });
    }

    // Supprime un élément du nœud
    bool deleteElement(Element<K, V>* element) {
        // Rechercher l'élément
        auto it = std::find_if(elements.begin(), elements.end(),
            [element](const Element<K, V>* e) {
                return *e == *element;
            });

        // Si l'élément est trouvé, le supprimer
        if (it != elements.end()) {
            delete *it;           // Libérer la mémoire
            elements.erase(it);   // Retirer de la collection
            return true;
        }

        return false;  // Élément non trouvé
    }

    // Opérateur de comparaison pour trier les nœuds
    bool operator<(const Node<K, V>& other) const {
        return key < other.key;
    }

    // Affichage (pour débogage)
    friend std::ostream& operator<<(std::ostream& os, const Node<K, V>& node) {
        os << "Node[key=" << node.key << ", elements=" << node.elements.size() << "]{";
        for (size_t i = 0; i < node.elements.size(); ++i) {
            if (i > 0) os << ", ";
            os << *node.elements[i];
        }
        os << "}";
        return os;
    }
};

#endif // NODE_H