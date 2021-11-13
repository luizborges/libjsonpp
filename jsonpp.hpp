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
#include <regex>

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
		void inline add(const std::string& name, const int value);
		void inline add(const std::string& name, const long long value);
		void inline add(const std::string& name, const size_t value);
		void inline add(const std::string& name, const bool value);

		/**
		 * Transforma para o formato json em uma string (stringify) um objeto que é o resultado de uma consulta sql do postgres.
		 * Esta função faz com que o resultado do sql do postegres, que representa uma matrix, seja transformado
		 * para uma string no formato json.
		 * Os valores sempre são gravados como um string.
		 * Esta função foi feita com fins de otimização, já que não é necessário criar um objeto intermediário apenas
		 * para passar os dados para o json.
		 * Esta função cria o seguinte objeto dentro do objeto json:
		 * @name = {
		 * "column_name" : ["column_name_1", ...., "column_name_n"],
		 * "data" : [ [ "data_1_1", ..., "data_1_n" ], ..., ["data_n_1", "data_n_n"] ]
		 * }
		 * Se não existe resultado, o resultado é vazio, é retornado o seguinte json:
		 * @name = {
		 * "column_name" : [],
		 * "data" : []
		 * }
		 * O campo "data" é uma matrix, que representa os valores retornados pelo sql.
		 * O campo "column_name" é um vetor que contém o nome das colunas retornados pelo sql, em ordem.
		 * ou seja, a posição da coluna 1 de "data" tem o nome da posição da coluna 1 do vetor "column_name".
		 * ou seja, a posição data[_][1] tem a coluna de nome column_name[1], e assim por diante.
		 * @arg name: nome do atributo que representa a string.
		 * @arg value: valor do atributo string representado pelo value.
		 */
		void add(const std::string& name, const pqxx::result& value);

		/**
		 * @brief Transforma um resultado de uma consulta do postgres em uma matrix para ser enviada em formato json.
		 * Esta função funciona exatamente igual a 'void add(const std::string& name, const pqxx::result& value);'
		 * com a diferença que esta sempre produzirá uma matrix que contém apenas 1 linha, já que o @arg(value) é uma linha.
		 * 
		 * Transforma para o formato json em uma string (stringify) um objeto que é o resultado de uma consulta sql do postgres.
		 * Esta função faz com que o resultado do sql do postegres, que representa uma linha (pqxx::row), seja transformado
		 * para uma string no formato json.
		 * Os valores sempre são gravados como um string.
		 * Esta função foi feita com fins de otimização, já que não é necessário criar um objeto intermediário apenas
		 * para passar os dados para o json.
		 * Esta função cria o seguinte objeto dentro do objeto json:
		 * @name = {
		 * "column_name" : ["column_name_1", ...., "column_name_n"],
		 * "data" : [ [ "data_1", ..., "data_n" ] ]
		 * }
		 * O data contém sempre apenas 1 linha.
		 * Se não existe resultado, o resultado é vazio, é retornado o seguinte json:
		 * @name = {
		 * "column_name" : [],
		 * "data" : []
		 * }
		 * O campo "data" é uma matrix, que representa os valores retornados pelo sql.
		 * Neste caso a matrix terá apenas 1 linha, que será a linha passada pelo @arg(value)
		 * O campo "column_name" é um vetor que contém o nome das colunas retornados pelo sql, em ordem.
		 * ou seja, a posição da coluna 1 de "data" tem o nome da posição da coluna 1 do vetor "column_name".
		 * ou seja, a posição data[_][1] tem a coluna de nome column_name[1], e assim por diante.
		 * @arg name: nome do atributo que representa a string.
		 * @arg value: valor do atributo string representado pelo value.
		 */
		void add(const std::string& name, const pqxx::row& value);

		/**
		 * Parser an array and returing the array in std:: structure
		 * Sempre é string o resultado interno do array.
		 * ARRAY_T = é uma estrutura STD que representa o arranjo, pode ser:
		 * std::vector, std::list, std::array, etc..
		 * É necessário passar somente a primeira parte pois sempre os dados serão retornados em forma de std::string.
		 * Logo a estrutura retornada devolve a seguinte estrutura:
		 * exemplos:
		 * ARRAY_T = std::vector => std::vector<std::string>
		 * ARRAY_T = std::list => std::list<std::string>
		 * A função pode ser utilizada da seguinte maneira:
		 * auto vector = parse_array<std::vector>("["val1","val2"]");
		 * A string JSON é repartida (token, split) pelo seguinte regex: std::regex e("\",\"");
		 * @OBS: insere os elementos na estrutura passada pela função "push_back()".
		 * Caso seja outra função a desejável, utilizar a função abaixo.
		 * @OBS: a seguinte entrada "[]" retorna uma estrutura ARRAY_T<std::string> vazia, sem a inserção de nenhum componente.
		 * A checagem para o array vazio é feita da seguinte forma:
		 * if(std::regex_match(@arg(str), std::regex("^\\[(\\s)*\\]$")))
		 * A checagem não foi feita na forma acima por questões de eficiência e porque quase nunca se encontrava um array vazio.
		 * Logo a notação de um array JSON vazio, segue estritamente a notação JSON, e a conferencia do array vazio é feito
		 * da seguinte forma:
		 * if(@arg(str) == "[]") return {};
		 * @OBS: na entrada é realizada a seguinte operação: const std::string str = trim(@arg(_str_));
		 * @OBS: o arranjo deve ser unidimensional para esta função funcionar, caso contrário lança uma exceção.
		 * @OBS: A parte de decode, visa possibilitar inserir em uma posição do array o valor da string delimitadora do array.
		 * A codificação e a decodificação são feitas por funções similares as funções javascript:
		 * encodeURIComponent() e decodeURIComponent()
		 * Esta parte não está implementada ainda.
		 * @arg str: string que JSON que representa um arranjo unidimensional.
		 * @arg decode: true -> aplica a função similar a decodeURIComponent() do javascript no valor de cada posição da arranjo.
		 * @return: uma estrutura STD que representa o array passado na string, com os valores JSON que representam o arranjo.
		 */
		template<template<typename> typename ARRAY_T>
		static ARRAY_T<std::string> parse_array(const std::string& _str_, const bool decode = false);

		/**
		 * Esta função é exatamente a mesma da função acima, com a diferença que é possível passar a função
		 * que insere, adiciona, os elementos na estrutura do array passado.
		 * Para utilizar esta função, basta fazer o seguinte: ex:
		 * auto vector = parse_array<std::vector, &std::vector<std::string>::push_back>("["val1", "val2"]");
		 * o argumento @param(ADD) é o ponteiro para a função que irá inserir, adicionar, os elementos na estrutura passada.
		 * A função deve satisfazer a interface de ser: void(ARRAY_T<std::string>::*ADD)(const std::string&).
		 * A função também deve ser um membro público da estrutura passada (ARRAY_T<std::string>).
		 */
		template<template<typename> typename ARRAY_T, void(ARRAY_T<std::string>::*ADD)(const std::string&)>
		static ARRAY_T<std::string> parse_array(const std::string& _str_, const bool decode = false);

		// template<template<typename> typename ARRAY_T, typename T, void(ARRAY_T<T>::*ADD)(const std::string&), T(*from_string_to)(const std::string&)>
		// static ARRAY_T<std::string> parse_array(const std::string& _str_, const bool decode = false);
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

////////////////////////////////////////////////////////////////////////////////
// add functions
////////////////////////////////////////////////////////////////////////////////
void jsonpp::jstr_t::add(const std::string& name, const std::string& value)
{ try {
    str += "\""+ name +"\":\"" + value +"\",";
 } catch (const std::exception &e) { throw err(e.what()); }
}

void jsonpp::jstr_t::add(const std::string& name, const int value)
{ try {
    str += "\""+ name +"\":" + std::to_string(value) +",";
 } catch (const std::exception &e) { throw err(e.what()); }
}

void jsonpp::jstr_t::add(const std::string& name, const long long value)
{ try {
    str += "\""+ name +"\":" + std::to_string(value) +",";
 } catch (const std::exception &e) { throw err(e.what()); }
}

void jsonpp::jstr_t::add(const std::string& name, const size_t value)
{ try {
    str += "\""+ name +"\":" + std::to_string(value) +",";
 } catch (const std::exception &e) { throw err(e.what()); }
}

void jsonpp::jstr_t::add(const std::string& name, const bool value)
{ try {
    str += "\""+ name +"\":" + u::to_str(value) +",";
 } catch (const std::exception &e) { throw err(e.what()); }
}
////////////////////////////////////////////////////////////////////////////////
// template functions
////////////////////////////////////////////////////////////////////////////////
template<template<typename> typename ARRAY_T, void(ARRAY_T<std::string>::*ADD)(const std::string&)>
ARRAY_T<std::string> jsonpp::jstr_t::parse_array(const std::string& _str_, const bool decode)
{ try {
	std::string str = u::trim(_str_); // copia a string - será modificada em: std::regex_token_iterator
    
	////////////////////////////////////////////////////////////////////////////////
	// check input
	////////////////////////////////////////////////////////////////////////////////
    if(str.empty()) throw err("The JSON string that represents array can not be empty.");
    if(str == "[]") return {};
    if(str[0] != '[' || str[1] != '\"' ||
    	str[str.size() -2] != '\"' || str.back() != ']') {
        throw err("The JSON string (str) that represents the unidimensional array is incorrect.\n"
		"str: \"%s\"", str.c_str());
    }
    if(decode) {
		throw err("'decode = true' is not implemented yet.");
	}
    
	////////////////////////////////////////////////////////////////////////////////
	// tokening string
	////////////////////////////////////////////////////////////////////////////////
    std::regex e ("\",\"");
    std::regex_token_iterator<std::string::iterator> token ( str.begin(), str.end(), e, -1 );
	std::regex_token_iterator<std::string::iterator> token_end;
    
    // já foi checado que o array JSON tem ao menos um elemento, nem que seja uma string vazia.   
    // remove os 2 primeiros characters da string - "[\""
    auto tcheck = token;
	if(++tcheck == token_end) { // trata o caso que o token tem apenas 1 elmeento
        std::string s = *token;
        s.erase(0, 2); // erase characters "[\"" - two first characters
        s.pop_back(); // erase character ']' - last character
        s.pop_back(); // erase character '\"' - penultimate character
        return {s};
    }

	////////////////////////////////////////////////////////////////////////////////
	// trata o primeiro elemento do array
	////////////////////////////////////////////////////////////////////////////////
	ARRAY_T<std::string> array;
	ARRAY_T<std::string> *parray = &array; // necessário para executar a função passada como parâmetro pelo template
    std::string s = *token;
    s.erase(0, 2); // erase characters "[\"" - two first characters
    (parray->*ADD)(s); // executa a função que insere, adiciona, um novo elemento no arranjo
    ++token;
    
	////////////////////////////////////////////////////////////////////////////////
	// trata os demais elementos do array
	////////////////////////////////////////////////////////////////////////////////
    for(; token != token_end; ++token) {
        auto tcheck = token;
        if(++tcheck == token_end) { // trata o último token
            std::string s = *token;
            s.pop_back(); // erase character ']' - last character
            s.pop_back(); // erase character '\"' - penultimate character
            (parray->*ADD)(s); // executa a função que insere, adiciona, um novo elemento no arranjo
        } else {
            (parray->*ADD)(*token); // executa a função que insere, adiciona, um novo elemento no arranjo
        }
    }
    
    return array;

	////////////////////////////////////////////////////////////////////////////////
	// 
	////////////////////////////////////////////////////////////////////////////////
	/*ARRAY_T<std::string> array;
    for(size_t i = 0; i < str.size(); ++i) {
        if(str[i] == '"') {
            std::string value = "";
            for(i += 1; i < str.size(); ++i) {
                if(str[i] == '"') break;
                else value += str[i];
            }
        	// (p->*func)();
            // array.push_back(value);
            ARRAY_T<std::string> *parray = &array; // necessário para executar a função passada como parâmetro pelo template
            (parray->*ADD)(value); // executa a função que insere, adiciona, um novo elemento no arranjo
        }
    }

	// if(decode) { // para decodificar os valores
	// }*/
 } catch (const std::exception &e) { throw err(e.what()); }
}


template<template<typename> typename ARRAY_T>
ARRAY_T<std::string> jsonpp::jstr_t::parse_array(const std::string& _str_, const bool decode)
{ try {
	std::string str = u::trim(_str_); // copia a string - será modificada em: std::regex_token_iterator
    
	////////////////////////////////////////////////////////////////////////////////
	// check input
	////////////////////////////////////////////////////////////////////////////////
    if(str.empty()) throw err("The JSON string that represents array can not be empty.");
    if(str == "[]") return {};
    if(str[0] != '[' || str[1] != '\"' ||
    	str[str.size() -2] != '\"' || str.back() != ']') {
        throw err("The JSON string (str) that represents the unidimensional array is incorrect.\n"
		"str: \"%s\"", str.c_str());
    }
    if(decode) {
		throw err("'decode = true' is not implemented yet.");
	}
    
	////////////////////////////////////////////////////////////////////////////////
	// tokening string
	////////////////////////////////////////////////////////////////////////////////
    std::regex e ("\",\"");
    std::regex_token_iterator<std::string::iterator> token ( str.begin(), str.end(), e, -1 );
	std::regex_token_iterator<std::string::iterator> token_end;
    
    // já foi checado que o array JSON tem ao menos um elemento, nem que seja uma string vazia.   
    // remove os 2 primeiros characters da string - "[\""
    auto tcheck = token;
	if(++tcheck == token_end) { // trata o caso que o token tem apenas 1 elmeento
        std::string s = *token;
        s.erase(0, 2); // erase characters "[\"" - two first characters
        s.pop_back(); // erase character ']' - last character
        s.pop_back(); // erase character '\"' - penultimate character
        return {s};
    }

	////////////////////////////////////////////////////////////////////////////////
	// trata o primeiro elemento do array
	////////////////////////////////////////////////////////////////////////////////
	ARRAY_T<std::string> array;
    std::string s = *token;
    s.erase(0, 2); // erase characters "[\"" - two first characters
    array.push_back(s);
    ++token;
    
	////////////////////////////////////////////////////////////////////////////////
	// trata os demais elementos do array
	////////////////////////////////////////////////////////////////////////////////
    for(; token != token_end; ++token) {
        auto tcheck = token;
        if(++tcheck == token_end) { // trata o último token
            std::string s = *token;
            s.pop_back(); // erase character ']' - last character
            s.pop_back(); // erase character '\"' - penultimate character
            array.push_back(s);
        } else {
            array.push_back(*token);
        }
    }
    
    return array;
 } catch (const std::exception &e) { throw err(e.what()); }
}


// template<template<typename> typename ARRAY_T, typename T, void(ARRAY_T<T>::*ADD)(const std::string&), T(*from_string_to)(const std::string&)>
// ARRAY_T<std::string> jsonpp::jstr_t::parse_array(const std::string& _str_, const bool decode = false)
// { try {
// 	std::string str = u::trim(_str_); // copia a string - será modificada em: std::regex_token_iterator
    
// 	////////////////////////////////////////////////////////////////////////////////
// 	// check input
// 	////////////////////////////////////////////////////////////////////////////////
//     if(str.empty()) throw err("The JSON string that represents array can not be empty.");
//     if(str == "[]") return {};
//     if(str[0] != '[' || str[1] != '\"' ||
//     	str[str.size() -2] != '\"' || str.back() != ']') {
//         throw err("The JSON string (str) that represents the unidimensional array is incorrect.\n"
// 		"str: \"%s\"", str.c_str());
//     }
//     if(decode) {
// 		throw err("'decode = true' is not implemented yet.");
// 	}
    
// 	////////////////////////////////////////////////////////////////////////////////
// 	// tokening string
// 	////////////////////////////////////////////////////////////////////////////////
//     std::regex e ("\",\"");
//     std::regex_token_iterator<std::string::iterator> token ( str.begin(), str.end(), e, -1 );
// 	std::regex_token_iterator<std::string::iterator> token_end;
    
//     // já foi checado que o array JSON tem ao menos um elemento, nem que seja uma string vazia.   
//     // remove os 2 primeiros characters da string - "[\""
//     auto tcheck = token;
// 	if(++tcheck == token_end) { // trata o caso que o token tem apenas 1 elmeento
//         std::string s = *token;
//         s.erase(0, 2); // erase characters "[\"" - two first characters
//         s.pop_back(); // erase character ']' - last character
//         s.pop_back(); // erase character '\"' - penultimate character
//         return {s};
//     }

// 	////////////////////////////////////////////////////////////////////////////////
// 	// trata o primeiro elemento do array
// 	////////////////////////////////////////////////////////////////////////////////
// 	ARRAY_T<std::string> array;
//     std::string s = *token;
//     s.erase(0, 2); // erase characters "[\"" - two first characters
//     array.push_back(s);
//     ++token;
    
// 	////////////////////////////////////////////////////////////////////////////////
// 	// trata os demais elementos do array
// 	////////////////////////////////////////////////////////////////////////////////
//     for(; token != token_end; ++token) {
//         auto tcheck = token;
//         if(++tcheck == token_end) { // trata o último token
//             std::string s = *token;
//             s.pop_back(); // erase character ']' - last character
//             s.pop_back(); // erase character '\"' - penultimate character
//             array.push_back(s);
//         } else {
//             array.push_back(*token);
//         }
//     }
    
//     return array;
//  } catch (const std::exception &e) { throw err(e.what()); }
// }

#endif // JSONPP_HPP

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////



































