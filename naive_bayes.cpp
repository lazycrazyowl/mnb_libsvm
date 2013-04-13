#include "naive_bayes.hpp"
#include <cmath>

MultinomialNaiveBayes::MultinomialNaiveBayes()
{
    //m_cats.set_deleted_key(UNDEFINED_CAT_ID);
}

void MultinomialNaiveBayes::add_doc(const doc_info_t &doc)
{
    cat_info_t &cat = m_cats[doc.cat];
    LOG("add_doc " << doc.cat);
    for (const feature_t &f : doc.doc)
    {
        const feat_prob_map_t::iterator &iter = cat.features.find(f.first);
        if (iter == cat.features.end())
            cat.features[f.first] = 1 + f.second;
        else
            iter->second += f.second;

        m_vocab.insert(f.first);
    }
}

void MultinomialNaiveBayes::finalize()
{
    size_t vocab_size = m_vocab.size();
    std::cout << "finalize " << vocab_size << std::endl;

    for (cats_map_t::value_type &cat_pair : m_cats)
    {
        LOG("finalize cat " << cat_pair.first);
        feature_value_t sum = vocab_size;
        for (const feature_t &feat : cat_pair.second.features)
            sum += feat.second;
        LOG("finalize sum " << sum);
        for (feat_prob_map_t::value_type &feat : cat_pair.second.features)
        {
            feat.second /= sum;
            LOG("finalize feat " << feat.first << " " << feat.second);
        }
    }
}

cat_id_t MultinomialNaiveBayes::classify(const doc_t &doc) const
{
    LOG("classify " << doc.size());
    cat_id_t best_cat = UNDEFINED_CAT_ID;
    feature_value_t best_prob = -1;
    for (const cats_map_t::value_type &cat_pair : m_cats)
    {
        feature_value_t cat_prob = 0;
        LOG("classify cat " << cat_pair.first);
        for (const feature_t &feat : doc)
        {
            const feat_prob_map_t::const_iterator &iter = cat_pair.second.features.find(feat.first);
            if (iter != cat_pair.second.features.end())
                cat_prob += std::log(iter->second);
        }
        LOG("classify prob " << cat_prob);
        if (cat_prob > best_prob)
        {
            best_cat = cat_pair.first;
            best_prob = cat_prob;
            LOG("classify best");
        }
    }
    return best_cat;
}
