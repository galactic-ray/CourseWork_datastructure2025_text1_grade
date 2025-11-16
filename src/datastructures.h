#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <QString>
#include <QStringList>
#include <QList>
#include <cmath>

// ===================== 数据结构 =====================
struct Grade {
    int id;
    QString name;
    QString major;
    int year;
    QString subject;
    double score;
};

template<typename T>
class SeqList {
    T* a; 
    size_t n, cap;
    
    void reserve(size_t nc){
        if(nc<=cap) return;
        T* b=new T[nc];
        for(size_t i=0;i<n;++i) b[i]=a[i];
        delete[] a; a=b; cap=nc;
    }
    
public:
    SeqList():a(nullptr),n(0),cap(0){}
    ~SeqList(){ delete[] a; }
    size_t size() const { return n; }
    T& operator[](size_t i){ return a[i]; }
    const T& operator[](size_t i) const { return a[i]; }
    void push_back(const T& v){ if(n==cap) reserve(cap?cap*2:16); a[n++]=v; }
    void insert(size_t pos, const T& v){
        if(pos>n) pos=n;
        if(n==cap) reserve(cap?cap*2:16);
        for(size_t i=n;i>pos;--i) a[i]=a[i-1];
        a[pos]=v; ++n;
    }
    bool remove_at(size_t pos){
        if(pos>=n) return false;
        for(size_t i=pos+1;i<n;++i) a[i-1]=a[i];
        --n; return true;
    }
    void clear(){ delete[] a; a=nullptr; n=0; cap=0; }
};

class SubjectIndex {
    struct Node { 
        Grade rec; 
        Node* next; 
        Node(const Grade& g):rec(g),next(nullptr){} 
    };
    struct Bucket { 
        QString subject; 
        Node* head; 
        Bucket():head(nullptr){} 
    };
    SeqList<Bucket> buckets;

    Bucket* get_bucket(const QString& s){
        for(size_t i=0;i<buckets.size();++i)
            if(buckets[i].subject==s) return &buckets[i];
        Bucket b; b.subject=s; buckets.push_back(b);
        return &buckets[buckets.size()-1];
    }
    
public:
    ~SubjectIndex(){ clear(); }
    void clear(){
        for(size_t i=0;i<buckets.size();++i){
            Node* p=buckets[i].head;
            while(p){ Node* q=p->next; delete p; p=q; }
            buckets[i].head=nullptr;
        }
    }
    void rebuild(const SeqList<Grade>& list){
        clear(); 
        for(size_t i=0;i<list.size();++i) add(list[i]);
    }
    void add(const Grade& g){
        Bucket* b=get_bucket(g.subject);
        Node* p=new Node(g); 
        p->next=b->head; 
        b->head=p;
    }
    bool remove_one(const Grade& g){
        for(size_t i=0;i<buckets.size();++i){
            if(buckets[i].subject!=g.subject) continue;
            Node** pp=&buckets[i].head;
            while(*pp){
                if((*pp)->rec.id==g.id &&
                   (*pp)->rec.subject==g.subject &&
                   (*pp)->rec.name==g.name &&
                   std::abs((*pp)->rec.score-g.score)<1e-9)
                { 
                    Node* del=*pp; 
                    *pp=del->next; 
                    delete del; 
                    return true; 
                }
                pp=&((*pp)->next);
            }
        }
        return false;
    }
    QList<Grade> getBySubject(const QString& s) const{
        QList<Grade> result;
        for(size_t i=0;i<buckets.size();++i){
            if(buckets[i].subject==s){
                const Node* p=buckets[i].head;
                while(p){ result.append(p->rec); p=p->next; }
                return result;
            }
        }
        return result;
    }
    QStringList getAllSubjects() const{
        QStringList result;
        for(size_t i=0;i<buckets.size();++i)
            result.append(buckets[i].subject);
        return result;
    }
};

#endif // DATASTRUCTURES_H

