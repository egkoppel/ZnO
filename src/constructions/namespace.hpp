#ifndef _ZNO_NAMESPACE_H
#define _ZNO_NAMESPACE_H

#include <map>
#include <string>
#include "../types/type.hpp"
#include "function_def.hpp"
#include <stdio.h>
#include <iostream>
#include <fmt/format.h>

namespace AST {
	class Namespace {
		protected:
		std::map<std::string, AST::TypeInstance> named_types;
		std::map<std::string, std::shared_ptr<AST::Function>> named_functions;
		std::map<std::string, std::shared_ptr<AST::Namespace>> namespaces;
		std::string name;

		public:
		Namespace(std::string name): name(name), named_types(), named_functions(), namespaces() {}
		~Namespace() {}
		virtual std::string get_name() { return name; }

		AST::TypeInstance get_type_by_name(std::string name);
		std::shared_ptr<AST::Function> get_function_by_name(std::string name);
		std::shared_ptr<AST::Namespace> get_namespace_by_name (std::string name);

		void add_type_with_name(AST::TypeInstance t, std::string name) {
			named_types.insert({name, t});
		}

		Namespace* operator <<(AST::TypeInstance t);

		Namespace* operator <<(std::shared_ptr<AST::Function> f);

		Namespace* operator <<(std::shared_ptr<AST::Namespace> n);

		std::shared_ptr<AST::MemoryLoc> get_var(std::string var) {
			if (named_functions.find(var) != named_functions.end()) {
				auto m = named_functions[var];
				return m;
			}
			return nullptr;
		}

		void codegen() {
			for (auto &n : namespaces) {
				n.second->codegen();
			}

			for (auto &func : named_functions) {
				func.second->codegen(nullptr);
			}
		}
	};
}

namespace Parser {
	struct NamespaceParseReturn {
		AST::Namespace* parsed_namespace;
		std::string next_identifier;
		int next_token;
	};
	NamespaceParseReturn parse_namespace(FILE* f);
}

#endif
