#include "Trove/Commands/Commands.h"
#include "Trove/Modules/Chat.h"
#include "lilly/string.hpp"
#include <format>
#include <stack>
#include <unordered_map>
#include <vector>

namespace Trove::Commands {
	constexpr auto MSG_USAGE = "Usage: /calc <expression>";
	constexpr auto MSG_SUCCESS_FORMAT = "{} = {} (0x{:X})";
	constexpr auto MSG_DIV_ZERO = "Division by zero is not allowed!";
	constexpr auto MSG_NEG_EXP = "Negative exponent not supported!";
	constexpr auto MSG_INVALID_OPERATION = "Invalid operation! Supported operations: +, -, *, /, ^, &, **";
	constexpr auto MSG_INVALID_EXPRESSION = "Invalid expression!";

	static void calcCommandHandler(const CommandData& data) {
		std::vector<Trove::String> args = data.parseArgs();
		if (args.empty()) {
			Trove::err(MSG_USAGE);
			return;
		}

		Trove::String expression;
		for (unsigned int i = 0; i < args.size(); ++i) {
			if (i > 0) expression += " ";
			expression += args[i];
		}

		static const std::unordered_map<Trove::String, int> opPrecedence{
			{ "**", 3 },
			{ "*", 2 },
			{ "/", 2 },
			{ "&", 2 },
			{ "+", 1 },
			{ "-", 1 },
			{ "^", 0 }
		};

		auto isOperator = [&](const Trove::String& token) {
			return opPrecedence.contains(token);
		};
		auto getPrecedence = [&](const Trove::String& op) {
			if (auto it = opPrecedence.find(op); it != opPrecedence.end()) {
				return it->second;
			}
			return -1;
		};

		auto applyOperation = [&](long long a, long long b, const Trove::String& op, Trove::String& errMsg) -> long long {
			if (op == "+") return a + b;
			if (op == "-") return a - b;
			if (op == "*") return a * b;
			if (op == "/") {
				if (b == 0) {
					errMsg = MSG_DIV_ZERO;
					return 0;
				}
				return a / b;
			}
			if (op == "^") return a ^ b;
			if (op == "&") return a & b;
			if (op == "**") {
				if (b < 0) {
					errMsg = MSG_NEG_EXP;
					return 0;
				}
				long long result = 1;
				for (; b; b--) result *= a;
				return result;
			}
			errMsg = MSG_INVALID_OPERATION;
			return 0;
		};

		std::stack<long long> values;
		std::stack<Trove::String> operators;
		Trove::String errMsg;

		auto applyTopOperator = [&]() -> bool {
			if (values.size() < 2) {
				errMsg = MSG_INVALID_EXPRESSION;
				return false;
			}
			long long b = values.top(); values.pop();
			long long a = values.top(); values.pop();
			Trove::String op = operators.top(); operators.pop();
			values.push(applyOperation(a, b, op, errMsg));
			return errMsg.empty();
		};

		for (const auto& t : args) {
			if (!isOperator(t)) {
				values.push(t);
			} else {
				while (!operators.empty() && getPrecedence(operators.top()) >= getPrecedence(t)) {
					if (!applyTopOperator()) {
						Trove::err(errMsg);
						return;
					}
				}
				operators.push(t);
			}
		}

		while (!operators.empty()) {
			if (!applyTopOperator()) {
				Trove::err(errMsg);
				return;
			}
		}

		if (values.size() != 1) {
			Trove::err(MSG_INVALID_EXPRESSION);
			return;
		}

		long long result = values.top();
		Trove::print(std::format(MSG_SUCCESS_FORMAT, expression, result, result));
	}

	static bool _reg_calcCommandHandler = Trove::Commands::defer("calc", calcCommandHandler);
}
