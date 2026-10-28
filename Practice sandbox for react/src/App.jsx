// import { useState } from "react";
// import Accordion from "./components/Accordion";
// import Modal from "./components/Modal";
// import Tooltip from "./components/Tooltip";
import AutoComplete from "./components/AutoComplete";

export default function App() {
  // const [isOpen, setIsOpen] = useState(false);

  return (
    // Page layout moved here from StarRating so the component stays reusable
    <div className="">
      {/* <StarRating noOfStars={10} />
       */}
      {/* <Accordion
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
      /> */}
      {/* <button
        onClick={() => setIsOpen(true)}
        className="rounded-md bg-blue-600 px-4 py-2 text-white hover:bg-blue-700"
      >
        Open Modal
      </button>
      <Modal isOpen={isOpen} onClose={() => setIsOpen(false)}>
        <h2 className="mb-2 text-xl font-semibold">Hello!</h2>
        <p className="mb-4 text-gray-600">
          This is a simple modal using React Portal.
        </p>
        <button
          onClick={() => setIsOpen(false)}
          className="rounded-md bg-gray-200 px-4 py-2 hover:bg-gray-300"
        >
          Close Modal
        </button>
      </Modal> */}

      {/* <Tooltip text="This is a tooltip">
        <button className="rounded-md bg-blue-600 px-4 py-2 text-white">
          Hover Me
        </button>
      </Tooltip> */}
      <AutoComplete items={["suresh", "ramesh", "mukesh"]} />
    </div>
  );
}
