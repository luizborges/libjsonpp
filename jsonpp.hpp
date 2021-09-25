/**
 *
 * @descripion: funciona como um header que contém todas as bibliotecas, é necessário apenas adicionar
 * essa biblioteca para ter acesso a todas a biblioteca.
 */
#ifndef JSONPP_HPP
#define JSONPP_HPP


////////////////////////////////////////////////////////////////////////////////
// Includes - default libraries - C
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes - default libraries - C++
////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>

#include <pqxx/pqxx>
////////////////////////////////////////////////////////////////////////////////
// Includes - system dependent libraries
////////////////////////////////////////////////////////////////////////////////
#if defined(unix) || defined(__unix) || defined(__unix__) || (defined (__APPLE__) && defined (__MACH__)) // Unix (Linux, *BSD, Mac OS X)

#endif

////////////////////////////////////////////////////////////////////////////////
// Includes - my libraries
////////////////////////////////////////////////////////////////////////////////
#include <util.hpp>
//#include <headers/stackTracer.h>
////////////////////////////////////////////////////////////////////////////////
// Includes - namespace
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// namespace
////////////////////////////////////////////////////////////////////////////////
namespace jsonpp {

	/**
	 * Class that represents a json object
	 */
	class jstr_t {
	 private:
		std::string str;

	 public:

		/**
		 * Retorna uma string que representa o objeto json
		 * @return: retorna o objeto json em forma de string
		 */
		std::string inline stringify();

		/**
		 * Insere dentro do objeto json um objeto tipo string.
		 * @arg name: nome do atributo que representa a string.
		 * @arg value: valor do atributo string representado pelo value.
		 */
		void inline add(const std::string& name, const std::string& value);

		/**
		 * Transforma para o formato json em uma string (stringify) um objeto que é o resultado de uma consulta sql do postgres.
		 * Esta função faz com que o resultado do sql do postegres, que representa uma matrix, seja transformado
		 * para uma string no formato json.
		 * Os valores sempre são gravados como um string.
		 * os campos criados são:
		 * @name é o valor do argumento @arg name
		 * @name_rows = número total de linhas do sql retornado.
		 * @name = matrix que representa.
		 * A matrix NÃO é ordenada pelas linhas, porém, é pelo nome das colunas.
		 * Ex: o pqxx::result retorna o seguinte resultado:
		 * colunas: "C1" e "C2"
		 * com os seguintes valores nas linhas: linha 1 = { 0, 3}; linha 2 = {45, -9}
		 * será gravado o seguinte: @name = my_matrix
		 * "my_matrix_total_row" : 2,
		 * "my_matrix" : {
		 *  "C1" : ["0", "45"],
		 *  "C2" : ["3", "-9"]
		 * } 
		 * @arg name: nome do atributo que representa a string.
		 * @arg value: valor do atributo string representado pelo value.
		 */
		void add(const std::string& name, const pqxx::result& value);
	};
}

////////////////////////////////////////////////////////////////////////////////
// Implementation of templates and inline functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class jstr_t
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// inline functions
////////////////////////////////////////////////////////////////////////////////
std::string inline jsonpp::jstr_t::stringify()
{ try {
    if(str.empty()) return "{}";

    // se existe algum resultado
    str.pop_back(); // remove the last character ','
    return "{" + str + "}";
 } catch (const std::exception &e) { throw err(e.what()); }
}

void inline jsonpp::jstr_t::add(const std::string& name, const std::string& value)
{ try {
    str = "\""+ name +"\":\"" + value +"\",";
 } catch (const std::exception &e) { throw err(e.what()); }
}






#endif // JSONPP_HPP

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


































