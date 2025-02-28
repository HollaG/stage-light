// import preactLogo from '../../assets/preact.svg';
// import './style.css';

// export function Home() {
// 	return (
// 		<div class="home">
// 			<a href="https://preactjs.com" target="_blank">
// 				<img src={preactLogo} alt="Preact logo" height="160" width="160" />
// 			</a>
// 			<h1>Get Started building Vite-powered Preact Apps </h1>
// 			<section>
// 				<Resource
// 					title="Learn Preact"
// 					description="If you're new to Preact, try the interactive tutorial to learn important concepts"
// 					href="https://preactjs.com/tutorial"
// 				/>
// 				<Resource
// 					title="Differences to React"
// 					description="If you're coming from React, you may want to check out our docs to see where Preact differs"
// 					href="https://preactjs.com/guide/v10/differences-to-react"
// 				/>
// 				<Resource
// 					title="Learn Vite"
// 					description="To learn more about Vite and how you can customize it to fit your needs, take a look at their excellent documentation"
// 					href="https://vitejs.dev"
// 				/>
// 			</section>
// 		</div>
// 	);
// }

// function Resource(props) {
// 	return (
// 		<a href={props.href} target="_blank" class="resource">
// 			<h2>{props.title}</h2>
// 			<p>{props.description}</p>
// 		</a>
// 	);
// }

import { useState } from 'react';
import { CGroup, Response, Slot } from '../../types/types';
import SlotComponent from '../../components/Slot/Slot';



export function HomePage() {
	const [serverIp, setServerIp] = useState<string>('http://192.168.1.1/api');
	// const { data, loading, error, refetch, abort } = useFetch<Response<Partial<Group>[]>>(
	//     `${serverIp}/groups`
	// );

	const [groupData, setGroupData] = useState<Response<Partial<CGroup>[]>>({ data: [] });
	const [controllerState, setControllerState] = useState<{
		groupIndex: number,
		slotIndex: number
	}>({
		groupIndex: -1,
		slotIndex: -1
	})

	const [activeGroup, setActiveGroup] = useState<CGroup>();


	const groupsValue: {
		value: string;
		label: string;
	}[] = groupData?.data.map((group) => ({
		value: group.id?.toString() || "",
		label: group.name || 'Unknown',
	})) || [];



	const onIpUpdate = (evt: React.ChangeEvent<HTMLInputElement>) => {
		setServerIp(evt.currentTarget.value);
		wrapper()
	}

	const wrapper = async () => {
		getGroups().then(() => getState());
	}

	const getGroups = async () => {
		return fetch(`${serverIp}/groups`)
			.then((response) => response.json())
			.then((data) => {
				// console.log(data);
				setGroupData(data);
			})

	}

	const getState = async () => {
		const res1 = await fetch(`${serverIp}`);
		const data1 = await res1.json();
		// console.log({ state: data1 });
		setControllerState({
			groupIndex: data1.groupIndex,
			slotIndex: data1.slotIndex
		})

		const res2 = await fetch(`${serverIp}/groups/${data1.groupIndex}`);
		const data2 = await res2.json();

		// console.log({ group: data2 });

		setActiveGroup(data2.data);


	}

	const onChangeGroup = (groupId: string) => { // groupId is just the index
		fetch(`${serverIp}/groups?select=${groupId}`)
			.then(res => res.json())
			.then((data) => {
				// TODO: handle error
				getState();
			})
	}

	console.log({ activeGroup })

	// console.log({ groupsValue });
	return (
		<div style={{ padding: "4rem" }}>
			<div>
				<div>

					<input
						// label="Server IP"
						// description="ESP Host IP"
						placeholder="192.168.x.x"
						value={serverIp}
						onChange={onIpUpdate}
					/>
					<button onClick={() => wrapper()} >
						Refresh
					</button>
				</div>
				<div>

					<select
						// label="Active group"
						value={controllerState.groupIndex !== undefined ? controllerState.groupIndex.toString() : ""}
						// data={groupsValue}
						onChange={(groupId) => {
							// setControllerState({
							//     groupIndex: parseInt(value),
							//     slotIndex: controllerState.slotIndex
							// })
							// onChangeGroup(groupId!)
						}}
					/>
				</div>

				<div>
					{activeGroup ? activeGroup.slots.map((slot, index) => <SlotComponent slot={slot} key={index} />) : "No slots saved yet!"}
				</div>
			</div>
		</div>
	);
}
