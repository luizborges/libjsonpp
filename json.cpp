#include "jsonpp.hpp"


////////////////////////////////////////////////////////////////////////////////
// public global variables
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// private functions - head
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class jstr_t
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// private functions
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// public functions
////////////////////////////////////////////////////////////////////////////////
/*
void jsonpp::jstr_t::add(const std::string& name, const pqxx::result& R)
{ try {
    // insere o número de linhas da matrix resultado no JSON
    str += "\""+ name +"_rows\":\"" + std::to_string(R.size()) +"\",";

    if(R.size() == 0) { // se o resultado do sql é vazio
        str += "\""+name+"\":{},";
        return;
    }

    // insere a matrix mesmo no JSON, se existe algum resultado
    str += "\""+name+"\":{";
    for(int col = 0; col < static_cast<int>(R.columns()); ++col) {
        str += "\""+ u::to_str(R[0][col].name()) + "\":["; // recebe o nome da coluna
        for(int row = 0; row < static_cast<int>(R.size()); ++row) {
            str += "\"";
            str += R[row][col].is_null() ? "" : R[row][col].as<std::string>(); // insere o resultado
            str += "\",";
        }
        str.pop_back(); // remove the last character ','
        str += "],";
    }
    str.pop_back(); // remove the last character ','
    str += "},";

 } catch (const std::exception &e) { throw err(e.what()); }
}
*/
void jsonpp::jstr_t::add(const std::string& name, const pqxx::result& R)
{ try {
    if(R.size() == 0) { // se o resultado do sql é vazio
        str += "\""+name+"\":{"
            "\"column_name\":[],"
            "\"data\":[]"
            "},";
        return;
    }

    str += "\""+name+"\":{"; // cria o objeto dentro do json que será utilizado

    // insere o nome das colunas no json
    str += "\"column_name\":[";
    for(auto const& column : R[0]) {
        str += "\""+ u::to_str(column.name()) + "\",";
    }
    str.pop_back(); // remove the last character ','
    str += "],";

    // insere a matrix de dados(valores) que representa a busca feita
    str += "\"data\":[";
    for(auto const& row : R) {
        str += "[";
        for(auto const& col : row) {
            str += "\"";
            str += col.is_null() ? "" : col.as<std::string>(); // insere o resultado
            str += "\",";
        }
        str.pop_back(); // remove the last character ','
        str += "],";
    }
    str.pop_back(); // remove the last character ','
    str += "]},";

 } catch (const std::exception &e) { throw err(e.what()); }
}

void jsonpp::jstr_t::add(const std::string& name, const pqxx::row& row)
{ try {
    if(row.size() == 0) { // se o resultado do sql é vazio
        str += "\""+name+"\":{"
            "\"column_name\":[],"
            "\"data\":[]"
            "},";
        return;
    }

    str += "\""+name+"\":{"; // cria o objeto dentro do json que será utilizado

    // insere o nome das colunas no json
    str += "\"column_name\":[";
    for(auto const& column : row) {
        str += "\""+ u::to_str(column.name()) + "\",";
    }
    str.pop_back(); // remove the last character ','
    str += "],";

    // insere a matrix de dados(valores) que representa a busca feita
    str += "\"data\":[[";
    for(auto const& col : row) {
            str += "\"";
            str += col.is_null() ? "" : col.as<std::string>(); // insere o resultado
            str += "\",";
    }
    str.pop_back(); // remove the last character ','
    str += "]]},";

 } catch (const std::exception &e) { throw err(e.what()); }
}


////////////////////////////////////////////////////////////////////////////////
// private functions 
////////////////////////////////////////////////////////////////////////////////



















