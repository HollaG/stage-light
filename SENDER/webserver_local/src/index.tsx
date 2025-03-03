import { render } from 'preact';
import { LocationProvider, Router, Route } from 'preact-iso';

import { Header } from './components/Header.jsx';
import { NotFound } from './pages/_404.jsx';
import './style.css';
// import './pico.min.css'
import { HomePage } from './pages/Home/index.js';
import UrlProvider, { UrlContext } from './context/UrlContext.js';

export function App() {
    return (
        <UrlProvider>

            <LocationProvider>
                <Header />
                <main>
                    <Router>
                        <Route path="/" component={HomePage} />
                        <Route default component={NotFound} />
                    </Router>
                </main>
            </LocationProvider>
        </UrlProvider>
    );
}

render(<App />, document.getElementById('app'));
