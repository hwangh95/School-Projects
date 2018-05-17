class ReverseClass a
    where deepReverse :: a -> a

instance ReverseClass Integer
    where deepReverse x = x

instance ReverseClass Double
    where deepReverse x = x

instance ReverseClass Char
    where deepReverse x = x

instance ReverseClass Bool
    where deepReverse x = x

instance ReverseClass a => ReverseClass [a]
    where deepReverse [] = []
          deepReverse (x:xs)  = (deepReverse xs)++[(deepReverse x)]
