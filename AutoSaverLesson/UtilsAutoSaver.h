#ifndef UTILSAUTOSAVER_H
#define UTILSAUTOSAVER_H

#include<QObject>
#include"AdapterTables.h"

class UtilsAutoSaver : public QObject
{
    Q_OBJECT
public:
    explicit UtilsAutoSaver(QObject *parent = nullptr);

    template<typename T>
    std::vector<std::unique_ptr<T>> deepCopy(const std::vector<std::unique_ptr<T>>& copy)
    {
        std::vector<std::unique_ptr<T>> result;
        result.reserve(copy.size());

        for (const auto& ptr : copy) {
            if (ptr) {
                auto cloned = ptr->clone();
                result.push_back(std::unique_ptr<T>(static_cast<T*>(cloned.release())));
            }
        }


        return result;

    }

};

#endif // UTILSAUTOSAVER_H
