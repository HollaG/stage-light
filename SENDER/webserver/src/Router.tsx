import { createBrowserRouter, createHashRouter, RouterProvider } from 'react-router-dom';
import { HomePage } from './pages/Home.page';

// const router = createBrowserRouter([
//   {
//     path: '/',
//     element: <HomePage />,
//   },
// ]);


const router = createHashRouter([
    {
        path: '/',
        element: <HomePage />,
    },
]);
export function Router() {
    return <RouterProvider router={router} />;
}
