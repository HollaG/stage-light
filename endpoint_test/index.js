const express = require("express");
const app = express();
const port = 3000;
const cors = require("cors");
app.use(cors());

app.get("/", (req, res) => {
    res.send("Hello World!");
});

app.get("/api", (req, res) => {
    res.json({
        groupIndex: 0,
        slotIndex: 0,
    });
});

app.get("/api/groups", (req, res) => {
    // if ?select parameter is present 
    // return only the selected group

    const select = req.query.select; // ID as string
    if (select) {
        // res.json({
        //     data: [
        //         {
        //             id: parseInt(select),
        //             name: `Group ${select}`,
        //         },
        //     ],
        // });
        res.json({
            status: 200
        })

        return
    }

    res.json({
        data: [
            {
                id: 0,
                name: "Group 1",
            },
            {
                id: 1,
                name: "Group 2",
            },
            { 
                id: 2,
                name: "Group 3",
            }
        ],
    });
});

app.get("/api/groups/:id", (req, res) => {

    const id = req.params.id;

    if (id == 0) { 
        res.json({data: {
            id: 0,
            name: "Group 0",
            slots: [
                {
                    id: 0,
                    light: {
                        light__type: 1,
                        r: 255,
                        g: 0,
                        b: 0,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 1,
                    light: {
                        light__type: 1,
                        r: 0,
                        g: 255,
                        b: 0,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 2,
                    light: {
                        light__type: 1,
                        r: 0,
                        g: 0,
                        b: 255,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 3,
                    light: {
                        light__type: 1,
                        r: 255,
                        g: 255,
                        b: 255,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 4,
                    light: {
                        light__type: 1,
                        r: 255,
                        g: 255,
                        b: 0,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 5,
                    light: {
                        light__type: 1,
                        r: 0,
                        g: 255,
                        b: 255,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                }
            ],
        }});

        return
    }

    if (id == 1) {
        res.json({data: {
            id: 1,
            name: "Group 1",
            slots: [
                {
                    id: 0,
                    light: {
                        light__type: 1,
                        r: 255,
                        g: 24,
                        b: 550,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 1,
                    light: {
                        light__type: 1,
                        r: 55,
                        g: 5,
                        b: 0,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 2,
                    light: {
                        light__type: 1,
                        r: 0,
                        g: 2,
                        b: 225,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
            ],
        }});

        return
    }

    if (id == 2) {
        res.json({data: {
            id: 2,
            name: "Group 2",
            slots: [
                {
                    id: 0,
                    light: {
                        light__type: 1,
                        r: 0,
                        g: 0,
                        b: 0,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 1,
                    light: {
                        light__type: 1,
                        r: 3,
                        g: 2,
                        b: 0,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
                {
                    id: 2,
                    light: {
                        light__type: 1,
                        r: 124,
                        g: 55,
                        b: 255,
                        w: 0,
                        ww: 0,
                        light__transition: 0,
                    },
                },
            ],
        }});

        return
    }

})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`);
});

// export interface CGroup {
//     id: number;
//     name: string;
//     slots: Slot[];
// }

// export interface Light {
//     light__type: number,
//     r: number,
//     g: number,
//     b: number,
//     w: number,
//     ww: number,
//     light__transition: number,
// }
// export interface Slot {
//     light: Light
//     id: number, // just the index for now
// }