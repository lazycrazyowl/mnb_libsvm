#include "naive_bayes.hpp"
#include "data.hpp"
#include "training.hpp"

#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/errors.hpp>

#include <boost/filesystem.hpp>

#include <string>

bool parse_options(int argc, char *argv[], uint &folds, std::string &data_file)
{
    bool show_help = false;
    
    boost::program_options::options_description opts("Possible options");
    opts.add_options()
        ("folds"
            , boost::program_options::value<uint>(&folds)->default_value(10)
            , "Amount of folds to cross-validate"
        )
        ("data_file"
            , boost::program_options::value<std::string>(&data_file)
            , "File with data in libsvm format"
        )
        ("help"
            , boost::program_options::value<bool>(&show_help)->default_value(false)
            , "Show this help"
        );
            
    try
    {
        
        boost::program_options::variables_map vm;
        boost::program_options::store(
            boost::program_options::parse_command_line(
                    argc
                , argv
                , opts)
            , vm);
        boost::program_options::notify(vm);
        
        if (show_help)
        {
            std::cout << opts << std::endl;
            return false;
        }
        
        return true;
    }
    catch (boost::program_options::error & e)
    {
        std::cout << opts << std::endl;
        return false;
    }
}

int main(int argc, char *argv[])
{
    uint folds = 10;
    std::string data_file = "";
    if (!parse_options(argc, argv, folds, data_file))
        return 0;
    
    if (!boost::filesystem::exists(data_file))
    {
        std::cout << "Data file does not exist!" << std::endl;
        return -1;
    }
    
    doc_list_t docs = load_data_file(data_file);
    std::sort(docs.begin(), docs.end(), doc_info_cat_asc);
    doc_list_t::iterator first_positive = std::lower_bound(docs.begin(), docs.end(), docs.front(), doc_info_cat_asc);
    std::random_shuffle(docs.begin(), first_positive);
    std::random_shuffle(first_positive, docs.end());

    cls_stat_t res = n_fold_cv<MultinomialNaiveBayes>(
          docs.begin()
        , first_positive
        , first_positive
        , docs.end()
        , folds );
    
    std::cout << "accuracy: " << res.accuracy << std::endl
              << "false positives rate: " << res.false_pos_rate << std::endl
              << "false negatives rate: " << res.false_pos_rate << std::endl
              << "precision: " << res.precision << std::endl
              << "recall: " << res.recall << std::endl
              << "f measure: " << res.f_measure << std::endl;

    return 0;
}