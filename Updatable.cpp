#ifndef UPDATABLE_H
#define UPDATABLE_H
class Updatable {
public:
    virtual void update(double time) = 0;
    virtual ~Updatable() = default;
};
#endif // UPDATABLE_H