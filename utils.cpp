template <class Iterator, class Distance>
constexpr void advance(Iterator &ite, Distance val)
{
    using category = typename std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category>)
    {
        ite = ite + val;
    }
    else
    {
        if (val > 0)
        {
            while (val > 0)
            {
                --val, ++ite;
            }
        }
        else if (val < 0)
        {
            while (val < 0)
            {
                ++val, --ite;
            }
        }
    }
}

template<class Iterator>
constexpr auto distance(Iterator first, Iterator last)
{
    using category = typename std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category>)
    {
        return last - first;
    }
    else
    {
        typename std::iterator_traits<Iterator>::difference_type result = 0;
        while (first != last)
        {
            ++result;
            ++first;
        }
        return result;  
    }
}
