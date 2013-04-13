#include "naive_bayes.hpp"
#include <cmath>

MultinomialNaiveBayes::MultinomialNaiveBayes()
{
    m_cats.set_deleted_key(UNDEFINED_CAT_ID);
}

void MultinomialNaiveBayes::add_doc(cat_id_t cid, const doc_t &doc)
{
    cat_info_t &cat = m_cats[cid];
    for (const feature_t &f : doc)
    {
        feat_prob_map_t::iterator &iter = cat.features.find(f.first);
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
    for (const cats_map_t::value_type &cat_pair : m_cats)
    {
        feature_value_t sum = vocab_size;
        for (const feature_t &feat : cat_pair.second.features)
            sum += feat.second;
        for (feature_t &feat : cat_pair.second.features)
            feat.second /= sum;
    }
}

cat_id_t MultinomialNaiveBayes::classify(const doc_t &doc) const
{
    cat_id_t best_cat = UNDEFINED_CAT_ID;
    feature_value_t best_prob = -1;
    for (const cats_map_t::value_type &cat_pair : m_cats)
    {
        feature_value_t cat_prob = 0;
        for (const feature_t &feat : doc)
        {
            feat_prob_map_t::iterator iter = cat_pair.second.features.find(feat.first);
            if (iter != cat_pair.second.features.end())
                cat_prob += std::log(iter->second);
        }
        if (cat_prob > best_prob)
        {
            best_cat = cat_pair.first;
            best_prob = cat_prob;
        }
    }
    return best_cat;
}
