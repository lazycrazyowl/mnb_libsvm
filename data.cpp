#include "data.hpp"

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <fstream>

doc_info_t load_doc(std::string &src)
{
    typedef boost::char_separator<char> token_func_type;
    typedef boost::tokenizer<token_func_type> tokenizer_type;
    typedef tokenizer_type::const_iterator token_iter_type;

    doc_info_t result;
    token_func_type sep(" ");
    tokenizer_type tokenizer(src, sep);
    token_iter_type iter = tokenizer.begin();
    result.cat = boost::lexical_cast<cat_id_t>(*iter);
    iter++;
    while (iter != tokenizer.end())
    {
        std::vector<std::string> parsed;
        boost::split(parsed, *iter, boost::is_any_of(":"));
        feature_t feat;
        feat.first = boost::lexical_cast<feature_id_t>(parsed[0]);
        feat.second = boost::lexical_cast<feature_value_t>(parsed[1]);
        result.doc.push_back(feat);
        iter++;
    }
    return result;
}

doc_list_t load_data_file(std::string &fname)
{
    doc_list_t result;
    std::ifstream f(fname);

    doc_id_t did = 0;
    while (f.good())
    {
        std::string buf;
        std::getline(f, buf);
        if (buf.size() > 0)
        {
            doc_info_t doc = load_doc(buf);
            doc.id = did;
            did++;
            //LOG(result.size() << " " << doc.cat);
            result.push_back(doc);
        }
    }

    return result;
}