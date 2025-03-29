#ifndef ELEMENT_H
#define ELEMENT_H

#include <iostream>

template <typename K, typename V>
class Element {
private:
    K key;   // Clé de recherche de l'élément
    V value; // Valeur associée à la clé

public:
    // Constructeur
    Element(const K& k, const V& v) : key(k), value(v) {}
    
    // Constructeur par défaut
    Element() = default;
    
    // Accesseurs
    const K& getKey() const { return key; }
    const V& getValue() const { return value; }
    
    // Mutateurs (si nécessaire)
    void setKey(const K& k) { key = k; }
    void setValue(const V& v) { value = v; }
    
    // Opérateurs de comparaison (utiles pour le tri)
    bool operator==(const Element<K, V>& other) const {
        return key == other.key && value == other.value;
    }
    
    bool operator<(const Element<K, V>& other) const {
        // Trie d'abord par clé, puis par valeur
        if (key == other.key) {
            return value < other.value;
        }
        return key < other.key;
    }
    
    // Affichage (pour le débogage et tests)
    friend std::ostream& operator<<(std::ostream& os, const Element<K, V>& element) {
        os << "Element[key=" << element.key << ", value=" << element.value << "]";
        return os;
    }
};

#endif // ELEMENT_H