import StarRating from "./components/starRating";

export default function App() {
  return (
    // Page layout moved here from StarRating so the component stays reusable
    <div className="w-full min-h-screen flex items-center justify-center">
      <StarRating noOfStars={10} />
    </div>
  );
}
