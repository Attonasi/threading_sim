#ifndef FRAMEWORK_STRUCTUREMANAGER_H
#define FRAMEWORK_STRUCTUREMANAGER_H

#include <map>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

template <class T> class StructureManager {

    protected:
        boost::uuids::random_generator gen;
        map<const char*, pair<T,pthread_mutex_t>> structures;
        pthread_mutex_t mapMutex;
        StructureManager();
        virtual ~StructureManager();

    public:
        virtual char* create();
        virtual void destroy(const char* identifier);
        virtual int size(const char *identifier);
};


template <class T> StructureManager<T>::StructureManager() {
    pthread_mutex_init(&mapMutex, NULL);
}

template <class T> StructureManager<T>::~StructureManager() {
    pthread_mutex_destroy(&mapMutex);
    for(auto iter = structures.begin(); iter != structures.end(); iter++) {
        pthread_mutex_destroy(&(iter->second.second));
    }
}

template <class T> char* StructureManager<T>::create() {
    string id = boost::uuids::to_string(gen());
    char* idPtr = new char[id.size()+1]();
    strcpy(idPtr, id.c_str());
    pthread_mutex_lock(&mapMutex);
    pthread_mutex_t innerMutex;
    pthread_mutex_init(&innerMutex,NULL);
    structures.insert(make_pair(idPtr, make_pair(T(),innerMutex)));
    pthread_mutex_unlock(&mapMutex);
    return idPtr;
}

template <class T> void StructureManager<T>::destroy(const char* identifier) {
    pthread_mutex_lock(&mapMutex);
    structures.erase(identifier);
    pthread_mutex_unlock(&mapMutex);
    delete[] identifier;
}

template <class T> int StructureManager<T>::size(const char* identifier) {
    if (structures.find(identifier) == structures.end()) return 0;
    pthread_mutex_lock(&structures[identifier].second);
    int ret =  structures[identifier].first.size();
    pthread_mutex_unlock(&structures[identifier].second);
    return ret;
}

#endif //FRAMEWORK_STRUCTUREMANAGER_H
