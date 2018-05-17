mkMultAdd :: (Num a) => a -> a -> (a -> a)
mkMultAdd a b = (\x -> a * x + b)
