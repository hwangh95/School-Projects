oddMultOf3(X) :-
    (integer(X) ->
        1 is X mod 2,
        0 is X mod 3
    ;   format(user_error,"ERROR: The given parameter is not an integer ~i",_), fail
    ).

prod_list([],0).

prod_list([X1,X2],Product):-
    Product is X1 * X2,!.

prod_list([H|T],Product):-
    prod_list(T,Factor),
    Product is Factor * H.

palindrome(List):-
    reverse(List,List).

secondMinCheck([],0).

secondMinCheck([H|T],NonNum):-
    (number(H)->
        secondMinCheck(T,NonNum)
    ;   NonNum = H
    ).

secondMin(List,Min2):-
    length(List,Len),
    (Len >= 2 ->
        secondMinCheck(List,NonNum),
        (number(NonNum)->
            min_list(List,Min1),
            delete(List,Min1,NewList),
            (NewList \= [] ->
                min_list(NewList,Min2)
            ;   format(user_error,"ERROR: List has fewer than two unique elements. ~i",_), fail
            )
        ;   format(user_error,"ERROR: \"~w\" is not a number.",NonNum), fail
        )
    ;   format(user_error,"ERROR: List has fewer than two unique elements. ~i",_), fail
    ).
segregate([],[],[]).   

segregate([H|T],Even,Odd):-
    (0 is H mod 2 ->
        segregate(T,E,Odd),
        append([H],E,Even)
    ;   segregate(T,Even,O),
        append([H],O,Odd)
    ).
   
bookends(Left,Right,List):-
    prefix(Left,List),
    reverse(Right,NewRight),
    reverse(List,NewList),
    prefix(NewRight,NewList).

subslice(_,[]):-
    fail.

subslice(List,[H|T]):-
    (prefix(List,[H|T]) ->
        true
    ;   subslice(List,T)  
    ).

shiftHelper(S,0,Shifted):-
    Shifted = S,!.

shiftHelper(List,N,Shifted):-
    [H|T] = List,
    N_1 is N-1,
    append(T,[H],S),
    shift(S,N_1,Shifted).

shift(List,N,Shifted):-
    length(List,Len),
    NewN is N mod Len,
    shiftHelper(List,NewN,Shifted).

sumOdds(0,_,0):-!.

sumOdds(X,Count,Odds):-
    C is Count+1,
    divmod(X,10, X_10,Xmod10),
    (1 is Count mod 2 ->
        sumOdds(X_10,C,O),
        Odds is O + Xmod10
    ;   sumOdds(X_10,C,Odds)
    ).

sumEvens(0,_,0):-!.

sumEvens(X,Count,Evens):-
    C is Count+1,
    divmod(X,10, X_10,Xmod10),
    (0 is Count mod 2 ->
        sumEvens(X_10,C,E),
        E_double is Xmod10*2,
        divmod(E_double,10,E_10,Emod10),
        Evens is E + E_10 + Emod10
    ;   sumEvens(X_10,C,Evens)
    ).

luhn(X):-
    (integer(X) ->
        sumOdds(X,1,Odds),
        sumEvens(X,1,Evens),
        Val is (Odds+Evens) mod 10,
        Val =:= 0
    ;   format(user_error,"ERROR: The given parameter is not an integer ~i",_), fail
    ).

path(X,Y):-
    (edge(X,Y) ->
        true
    ;   edge(X,C), path(C,Y)
    ),!.

cycle(X):-
    path(X,X).
