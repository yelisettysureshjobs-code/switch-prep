import { useEffect, useState } from "react";

const AutoComplete = () => {
  const [search, setSearch] = useState("");
  const [recipes, setRecipes] = useState(null);
  const [cache, setCache] = useState({});
  const [loading, setLoading] = useState(false);

  const fetchRecipes = async (signal) => {
    if (cache[search] !== undefined) {
      console.log("returned cache");
      setRecipes(cache[search]);
      return;
    }
    setLoading(true);
    console.log("called for ", search);
    const res = await fetch(
      `https://dummyjson.com/recipes/search?q=${search}`,
      { signal: signal },
    );
    const value = await res.json();
    setRecipes(value.recipes);
    setCache((prev) => ({
      ...prev,
      [search]: value.recipes,
    }));
    setLoading(false);
  };

  const handleSearchChange = (e) => {
    setSearch(e.target.value);
  };

  useEffect(() => {
    const ctrl = new AbortController();
    const timer = setTimeout(() => fetchRecipes(ctrl.signal), 500);
    return () => {
      clearTimeout(timer);
      ctrl.abort();
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [search]);
  return (
    <div className="w-screen flex flex-col items-center justify-center mt-10">
      <div>
        <input
          value={search}
          type="text"
          placeholder="Enter the value"
          className="border border-solid border-black p-2 rounded-md w-[500px] outline-none"
          onChange={handleSearchChange}
        />
      </div>
      {search ? (
        <div className="w-[500px] text-center border border-solid border-black rounded-md mt-2 max-h-96 overflow-y-auto">
          {loading ? (
            <div className="p-2  ">fetching data</div>
          ) : recipes?.length > 0 ? (
            recipes?.map((recipe) => {
              return (
                <span
                  className="block py-2 hover:bg-gray-100 rounded-md"
                  key={recipe.id}
                >
                  {recipe.name}
                </span>
              );
            })
          ) : (
            <div className="p-2">No Recipes Found</div>
          )}
        </div>
      ) : (
        <></>
      )}
    </div>
  );
};

export default AutoComplete;
