#include "naive_bayes.hpp"
#include "data.hpp"

#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/errors.hpp>

#include <string>

void parse_options(int argc, char *argv[], uint &folds, std::string &data_file)
{
    boost::program_options::options_description opts("Naive Bayes classifier with libsvm-like input");
    opts.add_options()
        ("folds"
            , boost::program_options::value<uint>(&folds)->default_value(10)
            , "Amount of folds to cross-validate"
        )
        ("data_file"
            , boost::program_options::value<std::string>(&data_file)
            , "File with data in libsvm format"
        );
    boost::program_options::variables_map vm;
    boost::program_options::store(
          boost::program_options::parse_command_line(
                argc
              , argv
              , opts)
        , vm);
    boost::program_options::notify(vm);
}

int main(int argc, char *argv[])
{
    uint folds = 10;
    std::string data_file = "";
    parse_options(argc, argv, folds, data_file);
    
    doc_list_t docs = load_data_file(data_file);
    return 0;
}