import Header from "./components/Header";
import "./style.sass";

import React, { useEffect, useState } from "react";
import { Container, Section } from "react-bulma-components";
import {
  BrowserRouter as Router, Route, Routes
} from "react-router-dom";
import Footer from "./components/Footer";
import Home from "./components/Home";
import Update from "./components/Update";

function App() {
  const [status, setStatus] = useState({
    "status": "unknown",
    "serialNumber": "unknown",
    "macAddress": "unknown",
    "version": "unknown",
    "ipAddress": "unknown",
    "hostname": "unknown"
  });

  useEffect(() => {
    setInterval(() => {
      const interval = fetch("/status")
        .then(res => res.json())
        .then(data => setStatus(data))
        .catch(err => console.error(err));

      return () => clearInterval(interval);
    }, 1000);
  }, []);

  return (
    <Router>
      <Header />
      <Section>
        <Container>
          <Routes>
            <Route path="/update" element={<Update {...status} />} />
            <Route path="/" element={<Home {...status} />} />
          </Routes>
        </Container>
      </Section>
      <Footer {...status} />
    </Router>
  );
}

export default App;
