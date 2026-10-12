import Accordion from "./components/Accordion";

export default function App() {
  return (
    // Page layout moved here from StarRating so the component stays reusable
    <div className="w-full min-h-screen flex items-center justify-center">
      {/* <StarRating noOfStars={10} />
       */}
      <Accordion
        items={[
          {
            key: "1",
            label: "Project Planning",
            description:
              "Define project scope, objectives, timelines, and key deliverables.",
          },
          {
            key: "2",
            label: "Resource Management",
            description:
              "Plan and allocate resources including manpower, equipment, and materials.",
          },
          {
            key: "3",
            label: "Task Execution",
            description:
              "Track ongoing activities, monitor task progress, and manage dependencies.",
          },
          {
            key: "4",
            label: "Quality & Compliance",
            description:
              "Ensure project activities meet defined quality standards, safety requirements, and compliance guidelines.",
          },
          {
            key: "5",
            label: "Project Reporting",
            description:
              "Generate detailed reports covering project progress, performance metrics, risks, and overall status.",
          },
          {
            key: "6",
            label: "Project Reporting",
            description:
              "Generate detailed reports covering project progress, performance metrics, risks, and overall status.",
          },
          {
            key: "7",
            label: "Project Reporting",
            description:
              "Generate detailed reports covering project progress, performance metrics, risks, and overall status.",
          },
        ]}
        disabled={false}
        openItemsKeys={["1", "3"]}
      />
    </div>
  );
}
