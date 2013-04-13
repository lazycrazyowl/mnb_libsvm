#ifndef MNB_LIBSVM_TRAINING_HPP
#define MNB_LIBSVM_TRAINING_HPP

#include "basic_types.hpp"
#include <cmath>
#include <stdexcept>

template<typename TClassifier, typename TIter>
void train(TClassifier &cls, TIter begin, TIter end)
{
    while (begin != end)
        cls.add_doc(*begin++);
    cls.finalize();
}

struct cls_stat_t
{
    cls_stat_t(float rw = 0, float rb = 0, float ww = 0, float wb = 0
        , float acc = 0, float fp = 0, float fn = 0, float prec = 0
        , float rec = 0, float fm = 0)
        : right_white(rw), right_black(rb), wrong_white(ww), wrong_black(wb)
        , accuracy(acc), false_pos_rate(fp), false_neg_rate(fn), precision(prec)
        , recall(rec), f_measure(fm)
    { }

    float right_white;
    float right_black;
    float wrong_white;
    float wrong_black;
    float accuracy;
    float false_pos_rate;
    float false_neg_rate;
    float precision;
    float recall;
    float f_measure;
    
    void calc()
    {
        float total_white = right_white + wrong_white;
        float total_black = right_black + wrong_black;
        float total = total_white + total_black;

        cls_stat_t result;
        accuracy = (right_white + right_black) / total;
        false_pos_rate = wrong_white / total_white;
        false_neg_rate = wrong_black / total_black;

        float rb_ww = right_black + wrong_white;
        precision = rb_ww > 0 ? (right_black / rb_ww) : 0;
        recall = right_black / total_black;

        float p_r = result.precision + result.recall;
        f_measure = p_r > 0 ? 2 * (result.precision * result.recall) / p_r : 0;
    }
    
    cls_stat_t operator +=(const cls_stat_t &other)
    {
        right_white += other.right_white;
        right_black += other.right_black;
        wrong_white += other.wrong_white;
        wrong_black += other.wrong_black;
        accuracy += other.accuracy;
        false_pos_rate += other.false_pos_rate;
        false_neg_rate += other.false_neg_rate;
        precision += other.precision;
        recall += other.recall;
        f_measure += other.f_measure;
        return *this;
    }
    
    cls_stat_t operator /=(float k)
    {
        accuracy /= k;
        false_pos_rate /= k;
        false_neg_rate /= k;
        precision /= k;
        recall /= k;
        f_measure /= k;
        return *this;
    }
};

template<typename TClassifier, typename TRAIter>
cls_stat_t classify(TClassifier &cls, TRAIter begin, TRAIter end)
{
    cls_stat_t result;
    while (begin != end)
    {
        cat_id_t cls_res = cls.classify(begin->doc);
        if (begin->cat == cls_res)
            if (begin->cat > 0)
                result.right_white += 1;
            else
                result.right_black += 1;
        else
            if (begin->cat > 0)
                result.wrong_white += 1;
            else
                result.wrong_black += 1;
        begin++;
    }

    return result;
}

template<typename TClassifier, typename TRAIter>
cls_stat_t n_fold_cv(TRAIter neg_begin, TRAIter neg_end, TRAIter pos_begin, TRAIter pos_end, uint folds = 10)
{
    LOG("nfold " << folds);
    uint neg_docs = std::distance(neg_begin, neg_end);
    int neg_step = std::ceil(neg_docs / (float)folds);
    
    uint pos_docs = std::distance(pos_begin, pos_end);
    int pos_step = std::ceil(pos_docs / (float)folds);
 
    if (folds > neg_docs || folds > pos_docs)
    {
        LOG("Number of folds is greater than number of docs!");
        throw std::logic_error("Number of folds is greater than number of docs!");
    }
        
    LOG("nfold steps " << neg_docs << " " << neg_step << " " << pos_docs << " " << pos_step);
    cls_stat_t result;
    for (int i = 0; i < folds; i++)
    {
        LOG("nfold i " << i);
        TRAIter neg_left = neg_begin + i * neg_step;
        TRAIter neg_right = ((i + 1) * neg_step < neg_docs) ? (neg_begin + (i + 1) * neg_step) : neg_end;
        
        LOG("nfold neg " << (neg_left - neg_begin) << " " << (neg_right - neg_begin));
        
        TRAIter pos_left = pos_begin + i * pos_step;
        TRAIter pos_right = ((i + 1) * pos_step < pos_docs) ? (pos_begin + (i + 1) * pos_step) : pos_end;
        
        LOG("nfold pos " << (pos_left - pos_begin) << " " << (pos_right - pos_begin));
        
        TClassifier cls;
        TRAIter buf = neg_begin;
        while (buf != neg_left) cls.add_doc(*buf++);
        buf = neg_right;
        while (buf != neg_end) cls.add_doc(*buf++);

        buf = pos_begin;
        while (buf != pos_left) cls.add_doc(*buf++);
        buf = pos_right;
        while (buf != pos_end) cls.add_doc(*buf++);
        
        cls_stat_t fold_stat = classify(cls, neg_left, neg_right);
        fold_stat += classify(cls, pos_left, pos_right);
        fold_stat.calc();
        result += fold_stat;
    }

    result /= folds;

    return result;
}
#endif // MNB_LIBSVM_TRAINING_HPP
