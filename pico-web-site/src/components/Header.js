import React from "react";
import { Container, Heading, Hero, Image, Navbar } from "react-bulma-components";
import { Link } from "react-router-dom";

class Header extends React.Component {
    render() {
        return <Hero className="hero is-primary">
            <Hero.Header>
                <Container>
                    <Navbar>
                        <Navbar.Item renderAs="div">
                            <Link to={"/"}>Home</Link>
                        </Navbar.Item>
                        <Navbar.Item renderAs="div">
                            <Link to={"/update"}>Update Controller</Link>
                        </Navbar.Item>
                    </Navbar>
                </Container>
            </Hero.Header>
            <Hero.Body>
                <Container>
                    <Image src="/logo.svg" size={128} className="is-pulled-right" style={{ "top": "-32px", "position": "relative" }} />
                    <Heading>
                        <Link to={"/"}>Dragon Lair Roof Controller</Link>
                    </Heading>
                    <Heading subtitle>
                        <a href="https://darkdragonsastro.com" target="_blank" rel="noreferrer">Dark Dragons Astronomy</a>
                    </Heading>
                </Container>
            </Hero.Body>
        </Hero>;
    }
}

export default Header;
