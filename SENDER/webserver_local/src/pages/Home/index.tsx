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

import { TargetedEvent, useEffect, useRef, useState } from 'react';
import { CGroup, Light, Response, Slot } from '../../types/types';
import SlotComponent from '../../components/Slot/Slot';
import { useUrl } from '../../context/UrlContext';

import classes from "./style.module.css"
import ColorPicker from '../../components/ColorPicker/ColorPicker';



export function HomePage() {
	// const [serverIp, setServerIp] = useState<string>('http://localhost:3000/api');
	const { serverIp, setServerIp } = useUrl()
	const [groupData, setGroupData] = useState<Response<Partial<CGroup>[]>>({ data: [] });
	const [webState, setWebState] = useState<{
		groupIndex: number,
		slotIndex: number
	}>({
		groupIndex: 0,
		slotIndex: 0
	})

	const [activeGroup, setActiveGroupOnWeb] = useState<CGroup>();

	// on launch, fetch the groups, THEN the state
	const getGroups = async () => {
		return fetch(`${serverIp}/groups`)
			.then((response) => response.json())
			.then((data) => {
				// console.log(data);
				setGroupData(data);
			})

	}

	/**
	 * Get the state of the controller.
	 * @returns 
	 */
	const getControllerState = async () => {
		const res1 = await fetch(`${serverIp}`);
		const data1 = await res1.json() as {
			groupIndex: number,
			slotIndex: number
		};


		return data1;
	}

	const getState = async () => {
		const res1 = await fetch(`${serverIp}`);
		const data1 = await res1.json();
		setWebState({
			groupIndex: data1.groupIndex,
			slotIndex: data1.slotIndex
		})

		return data1;
	}


	const getData = () => {
		// getGroups().then(() => getState());
		getGroups();
		loadGroup(webState.groupIndex);
	}

	useEffect(() => {
		getGroups().then(() => loadGroup(0)) // always set to default
	}, [])

	console.log({ controllerState: webState })

	const onGroupChange = (e: TargetedEvent<HTMLSelectElement, Event>) => {
		// setControllerState({
		//     ...controllerState,
		//     groupIndex: index
		// })
		const index = (e.currentTarget.value);
		console.log({ index })

		loadGroup(index, 0);


	}

	/**
	 * Fetches the group data from the server
	 * 
	 * @param index 
	 */
	const loadGroup = async (index: any, slotIndex?: number) => {
		const res = await fetch(`${serverIp}/groups/${index}`);
		const data = await res.json();
		console.log({ group: data });
		setActiveGroupOnWeb(data.data);
		setWebState({
			...webState,
			groupIndex: parseInt(index),
			slotIndex: slotIndex !== undefined ? slotIndex : webState.slotIndex
		});
	}

	/**
	 * Sends the current slot to the controller
	 */
	const sendChange = () => {
		// send the group change to the controller
		fetch(`${serverIp}`, {
			method: "POST",
			headers: {
				'Content-Type': 'application/json'
			},
			body: JSON.stringify({
				groupIndex: webState.groupIndex,
				slotIndex: webState.slotIndex
			})
		})
	}

	/**
	 * Synchronizes the web state with the controller state (downloaded)
	 */
	const syncHandler = () => {
		getControllerState().then((data) => loadGroup(data.groupIndex, data.slotIndex))
	}






	const groupsValue: {
		value: string;
		label: string;
	}[] = groupData?.data.map((group) => ({
		value: group.id?.toString() || "",
		label: group.name || 'Unknown',
	})) || [];


	const [addNewColor, setAddNewColor] = useState<string>("#000000");
	const r = hexToRgb(addNewColor).r;
	const g = hexToRgb(addNewColor).g;
	const b = hexToRgb(addNewColor).b;

	const lightTypeInputRef = useRef();
	const durationInputRef = useRef();
	const onColorChange = (color: string) => {
		setAddNewColor(color);
	}

	const onSave = () => {
		const light: Light = {
			light__type: Number((lightTypeInputRef.current as HTMLSelectElement).value),
			r,
			g,
			b,
			w: 0,
			ww: 0,
			light__transition: Number((durationInputRef.current as HTMLInputElement).value) || 0
		}

		const slot: Slot = {
			light,
			id: activeGroup?.slots.length || 0
		}

		// send the new slot to the controller
		fetch(`${serverIp}/slots`, {
			method: "POST",
			headers: {
				'Content-Type': 'application/json'
			},
			body: JSON.stringify({
				groupIndex: webState.groupIndex,
				slotIndex: slot.id,
				slot
			})
		}).then(res => {
			if (res.ok) {
				console.log("Added slot")
				getData();
			}
		})
	}

	return (
		<div>
			<div class={classes.wrapper}>
				<div class={classes.ipInput}>

					<input
						style={{ margin: 0 }}
						// label="Server IP"
						// description="ESP Host IP"
						placeholder="192.168.x.x"
						value={serverIp}
					// onChange={onIpUpdate}
					/>
					<button style={{ width: "175px" }} onClick={() => {
						// wrapper() 
					}} >
						Refresh me
					</button>
				</div>
				<div class={classes.groupSelect}>

					<select
						// label="Active group"
						value={webState.groupIndex}
						// data={groupsValue}
						onChange={onGroupChange}
						style={{ margin: 0 }}
					>
						{groupsValue.map((group, index) => <option value={group.value} key={index}>{group.label}</option>)}

					</select>
					<button onClick={sendChange}>
						Send
					</button>
					<button onClick={syncHandler}> Sync </button>
				</div>

				<p className={classes.labelText}> Active slots </p>
				<div>
					{activeGroup ? activeGroup.slots.map((slot, index) => <SlotComponent slot={slot} key={index} groupId={activeGroup.id} refresh={getData} setWebState={setWebState} webState={webState} />) : "No slots saved yet!"}
				</div>

				<p className={classes.labelText}> Add new slot </p>
				<article className={classes.addNew}>
					<ColorPicker onChange={onColorChange} />
					<p> Red: {r}, Green: {g}, Blue: {b}</p>
					<div>

						<label htmlFor="duration">Transition duration</label>
						<input ref={durationInputRef} type="number" name="duration" placeholder="Transition duration (ms)" aria-label="Transition Duration"></input>
					</div>
					<div>
						<label htmlFor="light_type">Light type</label>
						<select ref={lightTypeInputRef} name="light_type" aria-label="Select light type" required>

							<option selected value="0">Static</option>
							<option value="1">Chase</option>

						</select>
					</div>
					<button onClick={onSave}> Add </button>
				</article>
			</div>
		</div>
	);
}

// Hex to RGB
function hexToRgb(hex: string) {
	if (hex.length < 7) return { r: 0, g: 0, b: 0 };

	const r = parseInt(hex.substring(1, 3), 16);
	const g = parseInt(hex.substring(3, 5), 16);
	const b = parseInt(hex.substring(5, 7), 16);

	return { r, g, b };
}

