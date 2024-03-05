import clsx from 'clsx';
import Link from '@docusaurus/Link';
import Heading from '@theme/Heading';
import styles from './index.module.css';
import { ReactTyped } from 'react-typed';

function Title() {
  return (
    <div>
      <Heading as='h1' className={`hero__title ${styles.Title}`}>
        A {" "}
        <ReactTyped
          className={styles.TitleHighlighted}
          strings={[
            'simple',
            'lightweight',
            'straightforward'
          ]}
          typeSpeed={100}
          loop
          backSpeed={20}
          cursorChar='|'
          showCursor={true}
        />
        <br/>
        C/C++ unit testing framework
      </Heading>
    </div>
  );
}

function Subtitle() {
  return (
    <p className={`hero__subtitle ${styles.Subtitle}`}>
      You can’t unit test C, right?
    </p>
  );
}

function Button({ isPrimary, txt, path }) {
  return (
    <div className={styles.Button}>
      <Link className={`button button--${isPrimary ? 'primary' : 'secondary' } button--lg`} to={path}>
        {txt}
      </Link>
    </div>
  );
}

function ButtonPair() {
  return (
    <div className={ styles.ButtonPair }>
      <Button
        isPrimary
        txt='Get started'
        path='/docs/getting-started/2.1-getting-started'
      />
      <Button
        txt='Learn the basics >'
        path='/docs/overview/1.1-what-is-carbon'
      />
    </div>
  );
}

function Info() {
  return (
    <div className={styles.Info}>
      <Title />
      <Subtitle />
      <ButtonPair />
    </div>
  );
}

function Logo() {
  return (
    <img
      className={styles.Logo}
      src='/carbon/img/programming.svg' />
  );
}

export default function HomepageHeader() {
  return (
    <header className={ clsx('hero hero--secondary', styles.heroBanner) }>
      <div className={`container ${styles.Container}`}>
        <Info />
        <Logo />
      </div>
    </header>
  );
}
