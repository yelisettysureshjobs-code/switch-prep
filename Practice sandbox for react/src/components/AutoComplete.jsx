import { useEffect, useRef, useState } from "react";
import { debounce } from "lodash";

const AutoComplete = ({ items }) => {
  //   console.log("items", items);
  const inputRef = useRef(null);
  const [search, setSearch] = useState("");
  const [searchItems, setSearchItems] = useState(items);
  const handleOnChange = (e) => {
    debounce(() => {
      console.log(e.target.value);
      setSearch(e.target.value);
    }, 400);
  };

  useEffect(() => {
    // eslint-disable-next-line react-hooks/set-state-in-effect
    setSearchItems([...items].filter((item) => item.includes(search)));
  }, [items, search]);

  return (
    <div
      id="autocomplete-container"
      className="flex flex-col gap-y-2 w-screen items-center justify-center h-screen"
    >
      <input
        type="text"
        value={search}
        onChange={handleOnChange}
        id="auto-complete-input"
        placeholder="Enter the value"
        className="border border-solid border-black rounded-md p-2 inline w-fit fixed"
        ref={inputRef}
      />
      {search?.length > 0 ? (
        <div className="fixed top-[54%] border border-solid border-black rounded-md w-[198px]  flex flex-col items-center justify-between p-2">
          {searchItems.map((item) => (
            <div
              key={item}
              className="h-6 "
              onClick={(value) => {
                console.log(value.target.innerText);
                setSearch(value.target.innerText);
              }}
            >
              {item}
            </div>
          ))}
        </div>
      ) : (
        <div></div>
      )}
    </div>
  );
};

export default AutoComplete;
