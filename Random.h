#pragma once
#include <random>
#include <memory>
#include <assert.h>
#include "Logger.h"
#include "CoreStructs.h"
#include "MathModule.h"
#include <vector>

/// <summary>
/// The Random class is a wrapper for static methods that can be called anywhere you include
/// this header file.
/// 
/// You can call:
/// Random::GetObject() -- Will return a unique pointer to a random object, seeded with a random int
/// Random::GetObject(T seed) -- Will return a unique pointer seeded with a T where T can be int, unsigned int, float or double
/// 
/// Once you have the random object, you can get random numbers from it (any Type that you
/// pass, as long as is one of these: int, unsigned int, float or double).
/// 
/// So if you pass a double into the RandomObject::GetRandom(), you will get a double, and
/// so on.
/// 
/// 
/// </summary>

class Random
{
public:
    enum class DirectionAxisNumber {k_Four, k_Eight, k_Free};

    class RandomObject
    {

    public:
        template<typename t>
        RandomObject(t seed)
        {
            assert(typeid(t) == typeid(int) ||
                typeid(t) == typeid(unsigned int) ||
                typeid(t) == typeid(float) ||
                typeid(t) == typeid(double));

            m_engine.seed(seed);
        }

        template<typename t>
        t GetRandomValue(t min, t max)
        {
            //T can only be: int, unsigned int, float or double
            assert(typeid(t) == typeid(int) ||
                typeid(t) == typeid(unsigned int) ||
                typeid(t) == typeid(float) ||
                typeid(t) == typeid(double));

            if (typeid(t) == typeid(int))
            {
                std::uniform_int_distribution<int> dist(static_cast<int>(min), static_cast<int>(max));
                return static_cast<t>(dist(m_engine));
            }
            if (typeid(t) == typeid(unsigned int))
            {
                std::uniform_int_distribution<unsigned int> dist(static_cast<unsigned int>(min), static_cast<unsigned int>(max));
                return static_cast<t>(dist(m_engine));
            }
            else if (typeid(t) == typeid(float))
            {
                std::uniform_real_distribution<float> dist(static_cast<float>(min), static_cast<float>(max));
                return static_cast<t>(dist(m_engine));

            }
            else//double
            {
                std::uniform_real_distribution<double> dist(static_cast<double>(min), static_cast<double>(max));
                return static_cast<t>(dist(m_engine));
            }
        }

        Point GetRandomDirection(DirectionAxisNumber dirType)
        {
            Point result;


            switch (dirType)
            {
            case DirectionAxisNumber::k_Free:
            {
                std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

                result.x = dist(m_engine);
                result.y = dist(m_engine);
                break;
            }

            case DirectionAxisNumber::k_Four:
            {
                std::uniform_int_distribution<int> dist(0, 1);

                if (dist(m_engine) == 0)
                {
                    result.x = 0.0f;
                    result.y = dist(m_engine) == 0 ? 1.0f : -1.0f;
                }
                else
                {
                    result.y = 0.0f;
                    result.x = dist(m_engine) == 0 ? 1.0f : -1.0f;
                }

                break;
            }
            case DirectionAxisNumber::k_Eight:
            {
                std::uniform_int_distribution<int> dist(-1, 1);

                result.x = static_cast<float>(dist(m_engine));
                result.y = static_cast<float>(dist(m_engine));

                //Keep on trying until the result we get is not (0,0)
                while (result.x == 0.0f && result.y == 0.0f)
                {
                    result.x = static_cast<float>(dist(m_engine));
                    result.y = static_cast<float>(dist(m_engine));
                }

                result = MathModule::GetDirection(Point(0, 0), result);

                break;
            }
            }

            result = MathModule::Normalize(result);

            return result;
        }

        template<typename t>
        Point GetRandomPoint(t min, t max)
        {
            //T must be either int, unsigned int or float
            assert(typeid(t) == typeid(int) ||
                typeid(t) == typeid(unsigned int) ||
                typeid(t) == typeid(float));

            Point result;

            if (typeid(t) == typeid(int))
            {
                std::uniform_int_distribution<int> dist(static_cast<int>(min), static_cast<int>(max));
                result.x = static_cast<float>(dist(m_engine));
                result.y = static_cast<float>(dist(m_engine));
            }
            if (typeid(t) == typeid(unsigned int))
            {
                std::uniform_int_distribution<unsigned int> dist(static_cast<unsigned int>(min), static_cast<unsigned int>(max));
                result.x = static_cast<float>(dist(m_engine));
                result.y = static_cast<float>(dist(m_engine));
            }
            else if (typeid(t) == typeid(float))
            {
                std::uniform_real_distribution<float> dist(static_cast<float>(min), static_cast<float>(max));
                result.x = dist(m_engine);
                result.y = dist(m_engine);
            }

            return result;
        }

        template<typename vecItem>
        void RandomizeVector(std::vector<vecItem>* vecPtr)
        {
            vecItem tempItem;

            for (size_t i = 0; i < vecPtr->size(); ++i)
            {
                //We will store a random index other than i here
                size_t randomIndex = i;

                //Get a random index that is not i and conforms to vecPtr's size
                while (randomIndex == i)
                    randomIndex = static_cast<size_t>(GetRandomValue(0, (static_cast<int>(vecPtr->size()) - 1)));

                //Swap items
                tempItem = (*vecPtr)[i];
                (*vecPtr)[i] = (*vecPtr)[randomIndex];
                (*vecPtr)[randomIndex] = tempItem;
            }
        }

    private:
        std::mt19937 m_engine;
    };

public:
    template<typename t>
    static std::unique_ptr<RandomObject> GetObject(t seed)
    {
        return std::make_unique<RandomObject>(seed);
    }

    static std::unique_ptr<RandomObject> GetObject()
    {
        std::random_device randomDevice;
        int seed = randomDevice();

        return Random::GetObject<int>(seed);
    }
};