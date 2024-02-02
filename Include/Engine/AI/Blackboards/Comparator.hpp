#pragma once
#include <sstream>
#include <string>
#include "Blackboard.hpp"


template <typename U>
concept HasOperatorEqual = requires(const U& lhs, const U& rhs)
{
    lhs == rhs;
};

template <typename U>
concept HasOperatorNotEqual = requires(const U& lhs, const U& rhs)
{
    lhs != rhs;
};

template <typename U>
concept HasOperatorGreater = requires(const U& lhs, const U& rhs)
{
    lhs > rhs;
};

template <typename U>
concept HasOperatorGreaterEqual = requires(const U& lhs, const U& rhs)
{
    lhs >= rhs;
};


template <typename U>
concept HasOperatorLessEqual = requires(const U& lhs, const U& rhs)
{
    lhs <= rhs;
};


template <typename U>
concept HasOperatorLess = requires(const U& lhs, const U& rhs)
{
    lhs < rhs;
};

namespace AI
{
class Blackboard;

    /**
     * \brief -  The type of comparison that is going to be evaluated
     */
    enum class ComparisonType
    {
        EQUAL = 0,
        NOT_EQUAL = 1,
        LESS = 2,
        LESS_EQUAL = 3,
        GREATER =4 ,
        GREATER_EQUAL =5
    };

    class IComparator
    {
    public:

        virtual bool Evaluate(const Blackboard& blackboard) const { return false; }
        virtual ~IComparator() = default;
        virtual std::string ToString() const { return {}; }
    };

    template <typename T>
    class Comparator : public IComparator
    {
    public:
        Comparator(const std::string& comparison_key, const ComparisonType comparison_type, const T& value): m_comparisonKey(comparison_key),m_comparisonType(comparison_type),m_value(value)
        {
        }

        /**
         * \brief - Turns the comparator to a string- useful for serialization methods
         * \return - a string with properties in order: key, type of the value, comparison type as an integer, value
         */
        std::string ToString() const override;

        /**
         * \brief - Compare the value from the blackboard with this object's requirements, return the
         * result of the evaluation
         * \param blackboard - a reference to a blackboard instance
         * \return - whether the evaluation was successful or not
         */
        bool Evaluate(const Blackboard& blackboard) const override;
       

        std::string GetComparisonKey() const { return m_comparisonKey; }
        ComparisonType GetComparisonType() const { return m_comparisonType; }
        T GetValue() const{ return m_value; }
    private:
        std::string m_comparisonKey;
        ComparisonType m_comparisonType;
        T m_value;
    };

    template <typename T>
    std::string Comparator<T>::ToString() const
    {
        std::stringstream toReturn;
        toReturn << m_comparisonKey << " ";

        if (typeid(T) == typeid(std::string))
        {
            toReturn << "string"<< " ";
        }
        else
        {
            toReturn << typeid(T).name() << " ";
        }

        toReturn << static_cast<int>(m_comparisonType) << " ";
        toReturn << m_value << " ";
        return toReturn.str();
    }

    template <typename T>
    bool Comparator<T>::Evaluate(const Blackboard& blackboard) const
    {
        {
            switch (m_comparisonType)
            {
                case ComparisonType::EQUAL:
                    if constexpr (HasOperatorEqual<T>)
                    {
                        auto query = blackboard.TryGet<T>(m_comparisonKey);
                        if (query == nullptr) return false;
                        return *query == m_value;
                    }
                    else
                    {
                        return false;
                    }
                case ComparisonType::NOT_EQUAL:
                    if constexpr (HasOperatorNotEqual<T>)
                    {
                        auto query = blackboard.TryGet<T>(m_comparisonKey);
                        if (query == nullptr) return false;
                        return *query != m_value;
                    }
                    else
                    {
                        return false;
                    }
                case ComparisonType::LESS:
                    if constexpr (HasOperatorLess<T>)
                    {
                        auto query = blackboard.TryGet<T>(m_comparisonKey);
                        if (query == nullptr) return false;
                        return *query < m_value;
                    }
                    else
                    {
                        return false;
                    }
                case ComparisonType::LESS_EQUAL:
                    if constexpr (HasOperatorLessEqual<T>)
                    {
                        auto query = blackboard.TryGet<T>(m_comparisonKey);
                        if (query == nullptr) return false;
                        return *query <= m_value;
                    }
                    else
                    {
                        return false;
                    }
                case ComparisonType::GREATER:
                    if constexpr (HasOperatorGreater<T>)
                    {
                        auto query = blackboard.TryGet<T>(m_comparisonKey);
                        if (query == nullptr) return false;
                        return *query > m_value;
                    }
                    else
                    {
                        return false;
                    }
                case ComparisonType::GREATER_EQUAL:
                    if constexpr (HasOperatorGreaterEqual<T>)
                    {
                        auto query = blackboard.TryGet<T>(m_comparisonKey);
                        if (query == nullptr) return false;
                        return *query >= m_value;
                    }
                    else
                    {
                        return false;
                    }
                default:
                    return false;
            }
        };
    }
}

