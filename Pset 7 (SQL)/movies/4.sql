-- determine the number of movies with an IMDb rating of 10.0.
-- output a table with a single column and a single row containing the number of movies with a 10.0 rating.

SELECT COUNT(*)
FROM ratings
WHERE rating = 10.0;
