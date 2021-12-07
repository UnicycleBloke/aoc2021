namespace aoc {


template <typename T>
class range
{
    public:
        range(T from, T to, T step = 1)
        : m_from{from}
        , m_to{to}
        , m_step{step}
        {
        }

        range(T to)
        : range{T{}, to}
        {
        }

        class iter
        {
            public:
                iter(T pos, T step) 
                : m_pos{pos}
                , m_step{step} 
                {
                }

                iter& operator++()                 { m_pos += m_step; return *this; }
                T operator*()                      { return m_pos; }
                bool operator!=(const iter& other) { return m_pos*m_step < other.m_pos*m_step; }
                //bool operator==(const Iter& other) { return !(other == *this); }
        
            private:
                T m_pos;
                T m_step;
        };

        iter begin() { return iter{m_from, m_step}; }
        iter end()   { return iter{m_to, m_step}; }

    private:
        T m_from;
        T m_to;
        T m_step;
};


} // namespace aoc {
