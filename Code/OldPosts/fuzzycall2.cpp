FuzzyCall fuzzyObject; // esse � o objeto local
FuzzyCall* pFuzzy = &fuzzyObject; // ponteiro para esse mesmo objeto

( fuzzyObject.*pMethod )(); // [objeto] .* [ponteiro de m�todo]
( fuzzyObject->*pMethod )(); // [ponteiro para objeto] ->* [ponteiro de m�todo] 
